#include "buffer.h"

namespace es
{
	Buffer::Buffer(GLenum type)
		:mType(type),
		 mSize(0)
	{
		GLES_CHECK_ERROR(glGenBuffers(1, &mID));
		GLES_CHECK_ERROR(glBindBuffer(type, mID));
		GLES_CHECK_ERROR(glBindBuffer(type, 0));
	}

	Buffer::Buffer(GLenum type, GLenum usage, std::size_t size, void* data)
		:mType(type),
		 mSize(size)
	{
		GLES_CHECK_ERROR(glGenBuffers(1, &mID));
		GLES_CHECK_ERROR(glBindBuffer(type, mID));
		GLES_CHECK_ERROR(glBufferData(type, size, data, usage));
		GLES_CHECK_ERROR(glBindBuffer(type, 0));
	}

	Buffer::~Buffer()
	{
		GLES_CHECK_ERROR(glDeleteBuffers(1, &mID));
	}

	std::shared_ptr<Buffer> Buffer::createWithData(GLenum type, GLenum usage, std::size_t size, void* data)
	{
		return std::make_shared<Buffer>(type, usage, size, data);
	}

	void Buffer::bind()
	{
		GLES_CHECK_ERROR(glBindBuffer(mType, mID));
	}

	void Buffer::bindBase(GLuint index)
	{
		GLES_CHECK_ERROR(glBindBufferBase(mType, index, mID));
	}

	void Buffer::bindRange(int index, std::size_t offset, std::size_t size)
	{
		GLES_CHECK_ERROR(glBindBufferRange(mType, index, mID, offset, size));
	}

	void Buffer::unbind()
	{
		GLES_CHECK_ERROR(glBindBuffer(mType, 0));
	}

	void* Buffer::mapRange(GLenum access, std::size_t offset, std::size_t size)
	{
		GLES_CHECK_ERROR(glBindBuffer(mType, mID));
		GLES_CHECK_ERROR(void* ptr = glMapBufferRange(mType, offset, size, access));
		GLES_CHECK_ERROR(glBindBuffer(mType, 0));
		return ptr;
	}

	void Buffer::unMap()
	{
		GLES_CHECK_ERROR(glBindBuffer(mType, mID));
		GLES_CHECK_ERROR(glUnmapBuffer(mType));
		GLES_CHECK_ERROR(glBindBuffer(mType, 0));
	}

	void Buffer::setData(GLintptr offset, GLsizeiptr size, void* data)
	{
		GLES_CHECK_ERROR(glBindBuffer(mType, mID));
		GLES_CHECK_ERROR(glBufferSubData(mType, offset, size, data));
		GLES_CHECK_ERROR(glBindBuffer(mType, 0));
	}

	GLuint Buffer::getID() const
	{
		return mID;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	VertexBuffer::VertexBuffer(GLenum usage, std::size_t size, void* data) : Buffer(GL_ARRAY_BUFFER, usage, size, data)
	{

	}

	VertexBuffer::~VertexBuffer()
	{

	}

	std::shared_ptr<VertexBuffer> VertexBuffer::createWithData(GLenum usage, std::size_t size, void* data)
	{
		return std::make_shared<VertexBuffer>(usage, size, data);
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	ElementBuffer::ElementBuffer(GLenum usage, std::size_t size, void* data) : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage, size, data)
	{

	}

	ElementBuffer::~ElementBuffer()
	{

	}

	std::shared_ptr<ElementBuffer> ElementBuffer::createWithData(GLenum usage, std::size_t size, void* data)
	{
		return std::make_shared<ElementBuffer>(usage, size, data);
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	InstanceBuffer::InstanceBuffer(GLenum usage, std::size_t size, void* data) : Buffer(GL_ARRAY_BUFFER, usage, size, data)
	{

	}

	InstanceBuffer::~InstanceBuffer()
	{

	}

	std::shared_ptr<InstanceBuffer> InstanceBuffer::createWithData(GLenum usage, std::size_t size, void* data)
	{
		return std::make_shared<InstanceBuffer>(usage, size, data);
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	UniformBuffer::UniformBuffer(GLenum usage, std::size_t size, void* data) : Buffer(GL_UNIFORM_BUFFER, usage, size, data)
	{

	}

	UniformBuffer::UniformBuffer(GLenum usage, Program* program, const std::string& uniformBlockName, GLuint bindingPoint) : Buffer(GL_UNIFORM_BUFFER)
	{
		GLuint blockIndex = GLES_CHECK_ERROR(glGetUniformBlockIndex(program->getID(), uniformBlockName.c_str()));
		GLint blockSize;
		GLES_CHECK_ERROR(glGetActiveUniformBlockiv(program->getID(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize));
		mSize = blockSize;

		bind();
		GLES_CHECK_ERROR(glBufferData(mType, blockSize, nullptr, usage));
		unbind();
		bindBase(bindingPoint);
	}

	UniformBuffer::~UniformBuffer()
	{

	}

	std::unique_ptr<UniformBuffer> UniformBuffer::createWithData(GLenum usage, std::size_t size, void* data)
	{
		return std::make_unique<UniformBuffer>(usage, size, data);
	}

	std::unique_ptr<UniformBuffer> UniformBuffer::createWithData(GLenum usage, Program* program, const std::string& uniformBlockName, GLuint bindingPoint)
	{
		return std::make_unique<UniformBuffer>(usage, program, uniformBlockName, bindingPoint);
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	ShaderStorageBuffer::ShaderStorageBuffer(GLenum usage, std::size_t size, void* data) : Buffer(GL_SHADER_STORAGE_BUFFER, usage, size, data)
	{

	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{

	}

	ShaderStorageBuffer* ShaderStorageBuffer::createWithData(GLenum usage, std::size_t size, void* data)
	{
		ShaderStorageBuffer* buffer = new (std::nothrow) ShaderStorageBuffer(usage, size, data);
		if (buffer)
		{
			return buffer;
		}
		delete(buffer);
		return nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	VertexArray::VertexArray(VertexBuffer* vbo, ElementBuffer* ebo, std::size_t vertexSize, const std::vector<VertexAttrib>& attribs)
	{
		GLES_CHECK_ERROR(glGenVertexArrays(1, &mID));
		GLES_CHECK_ERROR(glBindVertexArray(mID));
		
		vbo->bind();
		if (ebo)
		{
			ebo->bind();
		}

		for (std::size_t i = 0; i < attribs.size(); i++)
		{
			GLES_CHECK_ERROR(glEnableVertexAttribArray(i));

			if (attribs[i].type == GL_INT)
			{
				GLES_CHECK_ERROR(glVertexAttribIPointer(i, attribs[i].numSubElements, attribs[i].type, vertexSize, (void*)((uint64_t)attribs[i].offset)));
			}
			else
			{
				GLES_CHECK_ERROR(glVertexAttribPointer(i, attribs[i].numSubElements, attribs[i].type, attribs[i].normalized, vertexSize, (void*)((uint64_t)attribs[i].offset)));
			}
		}
		mVertexAttribCount = attribs.size();

		GLES_CHECK_ERROR(glBindVertexArray(0));
		
		vbo->unbind();

		if (ebo)
		{
			ebo->unbind();
		}
	}

	VertexArray::~VertexArray()
	{
		GLES_CHECK_ERROR(glDeleteVertexArrays(1, &mID));
	}

	std::shared_ptr<VertexArray> VertexArray::createWithData(VertexBuffer* vbo, ElementBuffer* ebo, std::size_t vertexSize, const std::vector<VertexAttrib>& attribs)
	{
		return std::make_shared<VertexArray>(vbo, ebo, vertexSize, attribs);
	}

	void VertexArray::bind()
	{
		GLES_CHECK_ERROR(glBindVertexArray(mID));
	}

	void VertexArray::unbind()
	{
		GLES_CHECK_ERROR(glBindVertexArray(0));
	}

	GLuint VertexArray::getID() const
	{
		return mID;
	}

	GLuint VertexArray::getVertexAttribCount() const
	{
		return mVertexAttribCount;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	Renderbuffer::Renderbuffer(GLenum internalFormat, uint32_t w, uint32_t h)
	{
		mTarget = GL_RENDERBUFFER;
		mInternalFormat = internalFormat;
		mWidth = w;
		mHeight = h;

		GLES_CHECK_ERROR(glGenRenderbuffers(1, &mID));
		GLES_CHECK_ERROR(glBindRenderbuffer(mTarget, mID));
		GLES_CHECK_ERROR(glRenderbufferStorage(mTarget, mInternalFormat, mWidth, mHeight));
		GLES_CHECK_ERROR(glBindRenderbuffer(mTarget, 0));
	}

	Renderbuffer::~Renderbuffer()
	{
		GLES_CHECK_ERROR(glDeleteRenderbuffers(1, &mID));
	}

	std::unique_ptr<Renderbuffer> Renderbuffer::create(GLenum internalFormat, uint32_t w, uint32_t h)
	{
		return std::make_unique<Renderbuffer>(internalFormat, w, h);
	}

	void Renderbuffer::bind()
	{
		GLES_CHECK_ERROR(glBindRenderbuffer(mTarget, mID));
	}

	void Renderbuffer::unbind()
	{
		GLES_CHECK_ERROR(glBindRenderbuffer(mTarget, 0));
	}

	void Renderbuffer::resize(uint32_t w, uint32_t h)
	{
		mWidth = w;
		mHeight = h;

		bind();
		GLES_CHECK_ERROR(glRenderbufferStorage(mTarget, mInternalFormat, mWidth, mHeight));
		unbind();
	}

	GLenum Renderbuffer::getTarget() const
	{
		return mTarget;
	}

	GLuint Renderbuffer::getID() const
	{
		return mID;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	Framebuffer::Framebuffer()
	{
		GLES_CHECK_ERROR(glGenFramebuffers(1, &mID));
	}

	Framebuffer::~Framebuffer()
	{
		GLES_CHECK_ERROR(glDeleteFramebuffers(1, &mID));
	}

	std::unique_ptr<Framebuffer> Framebuffer::create()
	{
		return std::make_unique<Framebuffer>();
	}

	void Framebuffer::bind()
	{
		GLES_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mID));
	}

	void Framebuffer::unbind()
	{
		GLES_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::attachRenderTarget(uint32_t attachment, Texture2D* texture, uint32_t layer, uint32_t mipLevel, bool draw, bool read)
	{
		bind();
		GLES_CHECK_ERROR(glBindTexture(texture->getTarget(), texture->getID()));

		GLenum buf = GL_COLOR_ATTACHMENT0 + attachment;

		bool isExists = false;
		for (std::size_t i = 0; i < mAttachments.size(); i++)
		{
			if (mAttachments[i] == buf)
			{
				isExists = true;
				break;
			}
		}
		if (!isExists)
		{
			mAttachments.push_back(buf);
		}

		GLES_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, buf, texture->getTarget(), texture->getID(), mipLevel));
		
		if (draw)
		{
			GLES_CHECK_ERROR(glDrawBuffers(mAttachments.size(), mAttachments.data()));
		}
		else
		{
			GLES_CHECK_ERROR(glDrawBuffers(0, GL_NONE));
		}

		if (read)
		{
			GLES_CHECK_ERROR(glReadBuffer(buf));
		}
		else
		{
			GLES_CHECK_ERROR(glReadBuffer(GL_NONE));
		}

		checkStatus();

		glBindTexture(texture->getTarget(), 0);

	    unbind();
	}

	void Framebuffer::attachRenderTarget(uint32_t attachment, TextureCube* texture, uint32_t face, uint32_t mipLevel, bool draw, bool read)
	{
		bind();
		GLenum buf = GL_COLOR_ATTACHMENT0 + attachment;
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;

		bool isExists = false;
		for (std::size_t i = 0; i < mAttachments.size(); i++)
		{
			if (mAttachments[i] == buf)
			{
				isExists = true;
				break;
			}
		}
		if (!isExists)
		{
			mAttachments.push_back(buf);
		}

		GLES_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, buf, target, texture->getID(), mipLevel));

		if (draw)
		{
			GLES_CHECK_ERROR(glDrawBuffers(mAttachments.size(), mAttachments.data()));
		}
		else
		{
			GLES_CHECK_ERROR(glDrawBuffers(0, GL_NONE));
		}

		if (read)
		{
			GLES_CHECK_ERROR(glReadBuffer(buf));
		}
		else
		{
			GLES_CHECK_ERROR(glReadBuffer(GL_NONE));
		}

		checkStatus();

		unbind();
	}

	void Framebuffer::attachDepthRenderTarget(Texture* texture, uint32_t layer, uint32_t mipLevel)
	{
		bind();
		GLES_CHECK_ERROR(glBindTexture(texture->getTarget(), texture->getID()));

		GLES_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getTarget(), texture->getID(), mipLevel));

		GLES_CHECK_ERROR(glDrawBuffers(0, GL_NONE));

		GLES_CHECK_ERROR(glReadBuffer(GL_NONE));

		checkStatus();

		glBindTexture(texture->getTarget(), 0);

		unbind();
	}

	void Framebuffer::attachDepthRenderTarget(TextureCube* texture, uint32_t face, uint32_t layer, uint32_t mipLevel)
	{
		bind();

		GLES_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture->getID(), mipLevel));

		GLES_CHECK_ERROR(glDrawBuffers(0, GL_NONE));

		GLES_CHECK_ERROR(glReadBuffer(GL_NONE));

		checkStatus();

		unbind();
	}

	void Framebuffer::attachRenderBufferTarget(Renderbuffer* rbo)
	{
		bind();
		GLenum attachment;
		if (rbo->mInternalFormat == GL_DEPTH24_STENCIL8 || rbo->mInternalFormat == GL_DEPTH32F_STENCIL8)
		{
			attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		}
		else if (rbo->mInternalFormat == GL_DEPTH_COMPONENT || rbo->mInternalFormat == GL_DEPTH_COMPONENT16 ||
			rbo->mInternalFormat == GL_DEPTH_COMPONENT24 || rbo->mInternalFormat == GL_DEPTH_COMPONENT32F)
		{
			attachment = GL_DEPTH_ATTACHMENT;
		}
	
		GLES_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo->mID));
		checkStatus();
		unbind();
	}

	void Framebuffer::addAttachmentTextureLayer(GLenum attachment, GLuint texture, GLint level, GLint layer)
	{
		bind();

		GLES_CHECK_ERROR(glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture, level, layer));

		unbind();
	}

	void Framebuffer::addAttachmentTexture2D(GLenum attachment, GLenum texTarget, GLuint texture, GLint level)
	{
		bind();

		GLES_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, texture, level));
	
		unbind();
	}

	void Framebuffer::addAttachmentRenderbuffer(GLenum attachment, GLenum rboTarget, GLuint renderbuffer)
	{
		bind();

		GLES_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, rboTarget, renderbuffer));

		unbind();
	}

	void Framebuffer::drawBuffers(GLsizei n, const GLenum* bufs)
	{
		bind();

		GLES_CHECK_ERROR(glDrawBuffers(n, bufs));

		unbind();
	}

	void Framebuffer::readBuffers(GLenum src)
	{
		bind();

		GLES_CHECK_ERROR(glReadBuffer(src));

		unbind();
	}

	void Framebuffer::checkStatus()
	{
		GLenum status = GLES_CHECK_ERROR(glCheckFramebufferStatus(GL_FRAMEBUFFER));

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string error = "Framebuffer incomplete : ";

			switch (status)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				{
					error += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
					break;
				}
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				{
					error += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
					break;
				}
				case GL_FRAMEBUFFER_UNSUPPORTED:
				{
					error += "GL_FRAMEBUFFER_UNSUPPORTED";
					break;
				}
				default:
					break;
			}

			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, error.c_str());
		}
	}
}
