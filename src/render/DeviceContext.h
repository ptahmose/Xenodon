#ifndef _XENODON_RENDER_DEVICECONTEXT_H
#define _XENODON_RENDER_DEVICECONTEXT_H

#include <limits>
#include <cstdint>
#include <array>
#include <vulkan/vulkan.hpp>

struct Queue {
    vk::Queue queue;
    uint32_t family_index;

    Queue(vk::Device device, uint32_t family_index);
    Queue(std::nullptr_t);

    static Queue invalid();

    bool is_valid() const;
};

struct DeviceContext {
    vk::PhysicalDevice physical;
    vk::UniqueDevice logical;

    Queue graphics;
    Queue present;

    vk::UniqueCommandPool graphics_command_pool;

    DeviceContext(vk::PhysicalDevice physical, vk::ArrayProxy<const char* const> extensions, uint32_t graphics_queue, uint32_t present_queue);
};

#endif
