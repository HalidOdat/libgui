#include "Core/OpenGL.hpp"

#include "Renderer/FrameBuffer.hpp"

namespace Gui {

  namespace {
    constexpr static GLenum clearToOpenGLType(FrameBuffer::Clear clear) {
      GLenum openGLClear = 0;
      if (static_cast<u8>(clear) & static_cast<u8>(FrameBuffer::Clear::Color)) {
        openGLClear |= GL_COLOR_BUFFER_BIT;
      }
      if (static_cast<u8>(clear) & static_cast<u8>(FrameBuffer::Clear::Depth)) {
        openGLClear |= GL_DEPTH_BUFFER_BIT;
      }
      if (static_cast<u8>(clear) & static_cast<u8>(FrameBuffer::Clear::Stencil)) {
        openGLClear |= GL_STENCIL_BUFFER_BIT;
      }

      return openGLClear;
    }
  }

  FrameBuffer::Builder& FrameBuffer::Builder::clearColor(f32 r, f32 g, f32 b, f32 a) {
    mClearColor = Vec4(r, g, b, a);
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::clearColor(const Vec3& color) {
    mClearColor = Vec4(color, 1.0f);
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::clearColor(const Vec4& color) {
    mClearColor = color;
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::clear(FrameBuffer::Clear clear) {
    mClear = clear;
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::clearOnBind(bool yes) {
    mClearOnBind = yes;
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::attach(Attachment::Type type, Attachment::Format format, bool drawable, bool isMultisample) {
    mAttachments.push_back(FrameBuffer::Attachment{type, format, drawable, isMultisample});
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::attachDefaultDepthStencilBuffer() {
    mDepthStencilAttachment = {
      Attachment::Type::RenderBuffer,
      Attachment::Format::DefaultDepthStencil,
      true
    };
    return *this;
  }
  FrameBuffer::Builder& FrameBuffer::Builder::depthStencilType(Attachment::Type type) {
    mDepthStencilAttachment.type = type;
    return *this;
  }
  FrameBuffer::Handle FrameBuffer::Builder::build() {
    return FrameBuffer::create(*this);
  }
  FrameBuffer::Builder FrameBuffer::builder(u32 width, u32 height) {
    return FrameBuffer::Builder(width, height);
  }
  FrameBuffer::Handle FrameBuffer::create(Builder& builder) {
    return std::make_shared<FrameBuffer>(builder);
  }
  FrameBuffer::FrameBuffer(Builder& builder)
    : mId{0}
  {
    mWidth  = builder.mWidth;
    mHeight = builder.mHeight;

    mClearOnBind = builder.mClearOnBind;
    mClear       = builder.mClear;
    mClearColor  = builder.mClearColor;
    mAttachmentsSpecification = std::move(builder.mAttachments);
    mDepthStencilAttachmentSpecification = builder.mDepthStencilAttachment;

    invalidate(mWidth, mHeight);
  }
  void FrameBuffer::invalidate(u32 width, u32 height) {
    GUI_DEBUG_ASSERT(width != 0 && height != 0);

    mWidth  = width;
    mHeight = height;

    if (mId != 0) {
      destroy();
      mId = 0;
    }

    // Create and bind
    glGenFramebuffers(1, &mId);
    glBindFramebuffer(GL_FRAMEBUFFER, mId);

    u32 drawableAttachmentsCount = 0;
    GLenum drawableAttachments[32] = {};

    GUI_DEBUG_ASSERT(mAttachmentsSpecification.size() <= 32);

    for (u32 i = 0; i < mAttachmentsSpecification.size(); ++i) {
      Attachment& attachment = mAttachmentsSpecification[i];

      // TODO: implement other attachment types
      GUI_ASSERT(attachment.type == Attachment::Type::Texture);

      // TODO: implement multisampled attachment types
      GUI_ASSERT(!attachment.isMultisample);

      // create color attachment texture
      auto texture = Texture::buffer(width, height)
        .format(attachment.format)
        .filtering(Texture::FilteringMode::Linear)
        .mipmap(Texture::MipmapMode::None)
        .gammaCorrected(false)
        .build();

      // bind color attachment
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getId(), 0);

      if (attachment.drawable) {
        drawableAttachments[drawableAttachmentsCount++] = GL_COLOR_ATTACHMENT0 + i;
      }

      mColorAttachments.emplace_back(std::move(texture));
    }

    if (mDepthStencilAttachmentSpecification.type == Attachment::Type::RenderBuffer) {
      // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
      glGenRenderbuffers(1, &mDepthStencilAttachment);
      glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachment);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

      // bind depth and stencil attachment
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilAttachment);
    } else {
      mDepthStencilTexture = Texture::buffer(width, height)
        .format(mDepthStencilAttachmentSpecification.format)
        .filtering(Texture::FilteringMode::Linear)
        .mipmap(Texture::MipmapMode::None)
        .gammaCorrected(false)
        .build();

      mDepthStencilTexture->bind(0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);      

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilTexture->getId(), 0);
    }

    GUI_ASSERT_WITH_MESSAGE(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "framebuffer is not complete!"
    );

    glDrawBuffers(drawableAttachmentsCount, drawableAttachments);

    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::destroy() {
    glDeleteFramebuffers(1, &mId);
    mColorAttachments.clear();
    glDeleteRenderbuffers(1, &mDepthStencilAttachment);
    mDepthStencilTexture = {};
  }

  FrameBuffer::~FrameBuffer() {
    destroy();
  }

  void FrameBuffer::bind(bool forDraw) {
    if (forDraw) {
      glBindFramebuffer(GL_FRAMEBUFFER, mId);
      glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);

      if (mClearOnBind) {
        this->clear();
      }
    } else {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, mId);
    }
  }

  void FrameBuffer::clear() {
    glClear(clearToOpenGLType(mClear));
  }

  void FrameBuffer::clear(FrameBuffer::Clear clear) {
    glClear(clearToOpenGLType(clear));
  }

  void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

} // namespace Gui
