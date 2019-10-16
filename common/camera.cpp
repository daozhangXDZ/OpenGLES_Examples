#include "camera.h"

namespace es
{
	Camera::Camera(float fov, float near, float far, float aspectRatio, const glm::vec3& position, const glm::vec3& forward)
		:mNear(near),
		 mFar(far),
		 mAspectRatio(aspectRatio),
		 mPosition(position)
	{
		mForward = glm::normalize(forward);
		mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::normalize(glm::cross(mForward, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mForward));

		mRoll = 0.0f;
		mPitch = 0.0f;
		mYaw = 0.0f;

		mOrientation = glm::quat();
		mProjection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		mPrevViewProjection = glm::mat4(1.0f);

		update();
	}

	Camera::~Camera()
	{

	}

	std::unique_ptr<Camera> Camera::create(float fov, float near, float far, float aspectRatio, const glm::vec3& position, const glm::vec3& forward)
	{
		return std::make_unique<Camera>(fov, near, far, aspectRatio, position, forward);
	}

	void Camera::setTranslationDelta(glm::vec3 direction, float amount)
	{
		mPosition += direction * amount;
	}

	void Camera::setRotationDelta(glm::vec3 angle)
	{
		mYaw = glm::radians(angle.y);
		mPitch = glm::radians(angle.x);
		mRoll = glm::radians(angle.z);
	}

	void Camera::setPosition(glm::vec3 position)
	{
		mPosition = position;
	}

	void Camera::update()
	{
		glm::quat qPitch = glm::angleAxis(mPitch, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qYaw = glm::angleAxis(mYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat qRoll = glm::angleAxis(mRoll, glm::vec3(0.0f, 0.0f, 1.0f));

		mOrientation = qPitch * mOrientation;
		mOrientation = mOrientation * qYaw;
		mOrientation = qRoll * mOrientation;
		mOrientation = glm::normalize(mOrientation);

		mRotate = glm::mat4_cast(mOrientation);
		mForward = glm::conjugate(mOrientation) * glm::vec3(0.0f, 0.0f, -1.0f);

		mRight = glm::normalize(glm::cross(mForward, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mForward));

		mTranslate = glm::translate(glm::mat4(1.0f), -mPosition);
		mView = mRotate * mTranslate;
		mPrevViewProjection = mViewProjection;
		mViewProjection = mProjection * mView;

		frustumFromMatrix(mFrustum, mViewProjection);
	}

	void Camera::updateProjection(float fov, float near, float far, float aspectRatio)
	{
		mProjection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}

	bool Camera::aabbInsideFrustum(glm::vec3 maxV, glm::vec3 minV)
	{
		return true;
	}

	bool Camera::aabbInsidePlane(Plane plane, glm::vec3 maxV, glm::vec3 minV)
	{
		return true;
	}

	const glm::mat4& Camera::getView() const
	{
		return mView;
	}

	const glm::mat4& Camera::getProjection() const
	{
		return mProjection;
	}
}