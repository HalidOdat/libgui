#pragma once

#include "Core/Base.hpp"
#include "Renderer/Texture.hpp"

#include <vector>

namespace Gui {

  class FrameBuffer {
  public:
    using Handle = std::shared_ptr<FrameBuffer>;

    enum class Clear : u8 {
      Color   = 0b0000'0001,
      Depth   = 0b0000'0010,
      Stencil = 0b0000'0100,
    };

    struct Attachment {
      enum class Type : u8 {
        Texture,
        RenderBuffer,
      };

      using Format = Texture::Format;

      Type type;
      Format format;
      bool drawable      = true;
      bool isMultisample = false;
    };

    class Builder {
    public:
      Builder& clearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
      Builder& clearColor(const Vec3& color);
      Builder& clearColor(const Vec4& color);
      Builder& clear(FrameBuffer::Clear clear);
      Builder& clearOnBind(bool yes = true);
      Builder& attach(Attachment::Type type, Attachment::Format format, bool drawable = true, bool isMultisample = false);
      Builder& attachDefaultDepthStencilBuffer();
      Builder& depthStencilType(Attachment::Type type);
      FrameBuffer::Handle build();

    private:
      Builder(u32 width, u32 height)
        : mWidth{width}, mHeight{height}
      {}
 
    private:
      u32 mWidth;
      u32 mHeight;

      Vec4 mClearColor = {1.0f, 1.0f, 1.0f, 1.0f};
      FrameBuffer::Clear mClear = FrameBuffer::Clear::Color;
      bool mClearOnBind = true;
 
      std::vector<Attachment> mAttachments;
      Attachment mDepthStencilAttachment = {
        Attachment::Type::RenderBuffer,
        Attachment::Format::DefaultDepthStencil,
        false,
      };

    private:
      friend class FrameBuffer;
    };

  public:
    [[nodiscard]] static Builder builder(u32 width, u32 height);
    DISALLOW_MOVE_AND_COPY(FrameBuffer);
    ~FrameBuffer();

    void invalidate(u32 width, u32 height);

    void bind(bool forDraw = true);
    void unbind();

    void clear();
    void clear(FrameBuffer::Clear clear);

    inline const Texture::Handle& getColorAttachment(u32 index = 0) const {
      return mColorAttachments[index];
    }

    inline const Texture::Handle& getDepthAttachment() const {
      return mDepthStencilTexture;
    }

    inline u32 getId() const { return mId; }

  public:
    // DO NOT USE! Use the builder!
    //
    // NOTE: It has to be public so it can be constructed by std::make_shared.
    FrameBuffer(Builder& builder);

  private:
    static FrameBuffer::Handle create(Builder& builder);
    void destroy();

  private:
    u32 mId;
    bool mClearOnBind;
    FrameBuffer::Clear mClear;
    Vec4 mClearColor;

    std::vector<Attachment> mAttachmentsSpecification;
    Attachment mDepthStencilAttachmentSpecification;

    std::vector<Texture::Handle> mColorAttachments;
    Texture::Handle mDepthStencilTexture;
    u32 mDepthStencilAttachment = 0;

    u32 mWidth;
    u32 mHeight;
  };

  constexpr FrameBuffer::Clear operator|(FrameBuffer::Clear lhs, FrameBuffer::Clear rhs) {
    return static_cast<FrameBuffer::Clear>(
      static_cast<u8>(lhs) | static_cast<u8>(rhs)
    );
  }

} // namespace Gui
