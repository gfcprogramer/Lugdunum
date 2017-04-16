#pragma once

#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light/Light.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>
#include <lug/System/Clock.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

class LUG_GRAPHICS_API Forward final : public Technique {
private:
    struct DepthBuffer {
        std::unique_ptr<API::Image> image;
        std::unique_ptr<API::ImageView> imageView;
    };

    struct FrameData {
        DepthBuffer depthBuffer;
        API::Framebuffer frameBuffer;
        API::Fence fence;

        // There is actually only 1 command buffer
        std::vector<API::CommandBuffer> cmdBuffers;

        std::vector<BufferPool::SubBuffer*> freeSubBuffers;
    };

public:
    Forward(const Renderer& renderer, const View* renderView, const API::Device* device, API::Queue* presentQueue);

    Forward(const Forward&) = delete;
    Forward(Forward&&) = delete;

    Forward& operator=(const Forward&) = delete;
    Forward& operator=(Forward&&) = delete;

    ~Forward() = default;

    bool render(const ::lug::Graphics::Render::Queue& renderQueue, const API::Semaphore& imageReadySemaphore, const API::Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) override final;
    bool init(API::DescriptorPool* descriptorPool, const std::vector<std::unique_ptr<API::ImageView>>& imageViews) override final;
    void destroy() override final;

    bool initDepthBuffers(const std::vector<std::unique_ptr<API::ImageView>>& imageViews) override final;
    bool initFramebuffers(const std::vector<std::unique_ptr<API::ImageView>>& imageViews) override final;

private:
    std::unique_ptr<BufferPool> _cameraPool;
    std::unique_ptr<BufferPool> _lightsPool;

    std::unique_ptr<API::DeviceMemory> _depthBufferMemory{nullptr};

    std::unordered_map<Light::Light::Type, std::unique_ptr<API::Pipeline>> _pipelines;

    std::vector<FrameData> _framesData;

    std::unordered_map<std::string, BufferPool::SubBuffer*> _subBuffers;
};

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug