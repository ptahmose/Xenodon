#include "present/xorg/XorgSurface.h"
#include <stdexcept>
#include <algorithm>
#include <array>
#include <vector>
#include <cstring>
#include <iostream>
#include "present/xorg/XorgWindow.h"

namespace {
    constexpr const std::array DEVICE_EXTENSIONS = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct GpuInfo {
        vk::PhysicalDevice gpu;
        const char* device_name;
        uint32_t graphics_queue_index;
        uint32_t present_queue_index;
        uint32_t index;
    };

    GpuInfo pick_gpu(vk::Instance instance, vk::SurfaceKHR surface) {
        auto gpus = instance.enumeratePhysicalDevices();
        auto gpu_infos = std::vector<std::pair<vk::PhysicalDevice, vk::PhysicalDeviceProperties>>(gpus.size());

        std::transform(gpus.begin(), gpus.end(), gpu_infos.begin(), [](vk::PhysicalDevice gpu) {
            return std::pair {
                gpu,
                gpu.getProperties()
            };
        });

        // Make sure discrete GPUs are appearing before integrated GPUs, because
        // we always want to chose discrete over integrated.
        std::sort(gpu_infos.begin(), gpu_infos.end(), [](const auto& l, const auto& r) {
            auto rate = [](const auto& gpu) {
                switch (gpu.second.deviceType) {
                    case vk::PhysicalDeviceType::eDiscreteGpu:
                        return 0;
                    case vk::PhysicalDeviceType::eIntegratedGpu:
                        return 1;
                    default:
                        return 2;
                }
            };

            return rate(l) < rate(r);
        });

        auto graphics_supported = std::vector<uint32_t>();
        auto present_supported = std::vector<uint32_t>();
        for (size_t i = 0; i < gpu_infos.size(); ++i) {
            auto& [gpu, props] = gpu_infos[i];

            // Because we sorted earlier, if the type isnt one of these there arent
            // any more candidates, so we can exit early
            if (props.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
                props.deviceType != vk::PhysicalDeviceType::eIntegratedGpu)
                break;

            auto queue_family_properties = gpu.getQueueFamilyProperties();
            auto extension_properties = gpu.enumerateDeviceExtensionProperties();

            // Check if the GPU supports all required extensions
            {
                auto it = DEVICE_EXTENSIONS.begin();
                bool valid = true;
                for (; it != DEVICE_EXTENSIONS.end() && valid; ++it) {
                    auto cmp_ext = [&](auto& ext) {
                        return std::strcmp(*it, ext.extensionName) == 0;
                    };

                    valid = std::find_if(extension_properties.begin(), extension_properties.end(), cmp_ext) != extension_properties.end();
                }

                if (!valid)
                    continue;
            }

            // Check if the GPU supports the surface at all
            {
                uint32_t format_count, present_mode_count;
                vk::createResultValue(gpu.getSurfaceFormatsKHR(surface, &format_count, static_cast<vk::SurfaceFormatKHR*>(nullptr)), __PRETTY_FUNCTION__);
                vk::createResultValue(gpu.getSurfacePresentModesKHR(surface, &present_mode_count, static_cast<vk::PresentModeKHR*>(nullptr)), __PRETTY_FUNCTION__);

                if (format_count == 0 || present_mode_count == 0)
                    continue;
            }

            // Find a compute and present queue for of the gpu
            {
                graphics_supported.clear();
                present_supported.clear();
                auto queue_families = gpu.getQueueFamilyProperties();

                uint32_t num_queues = static_cast<uint32_t>(queue_families.size());
                for (uint32_t i = 0; i < num_queues; ++i) {
                    if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics)
                        graphics_supported.push_back(i);

                    if (gpu.getSurfaceSupportKHR(i, surface))
                        present_supported.push_back(i);
                }

                if (graphics_supported.empty() || present_supported.empty())
                    continue;

                auto git = graphics_supported.begin();
                auto pit = present_supported.begin();

                // Check if theres a queue with both supported
                while (git != graphics_supported.end() && pit != present_supported.end()) {
                    uint32_t g = *git;
                    uint32_t p = *pit;

                    if (g == p)
                        return {gpu, props.deviceName, g, p, static_cast<uint32_t>(i)};
                    else if (g < p)
                        ++git;
                    else
                        ++pit;
                }

                // No queue with both supported, but both are supported, so just take the first of both
                return {gpu, props.deviceName, graphics_supported[0], present_supported[0], static_cast<uint32_t>(i)};
            }
        }

        throw std::runtime_error("Failed to find a suitable physical device");
    }

    vk::UniqueSurfaceKHR create_surface(vk::Instance instance, XorgWindow& window) {
        auto [connection, xid] = window.x_handles();
        auto create_info = vk::XcbSurfaceCreateInfoKHR(
            {},
            connection,
            xid
        );

        return instance.createXcbSurfaceKHRUnique(create_info);
    }

    Device create_device(vk::Instance instance, vk::SurfaceKHR surface) {
        auto [gpu, name, gqi, pqi, index] = pick_gpu(instance, surface);
        std::cout << "Picked GPU " << index << ": " << name << std::endl;
        return Device(gpu, DEVICE_EXTENSIONS, gqi, pqi);
    }
}

XorgSurface::XorgSurface(vk::Instance instance, XorgWindow& window):
    surface(create_surface(instance, window)),
    device(create_device(instance, this->surface.get())) {
}
