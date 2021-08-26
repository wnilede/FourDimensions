#include <cmath>
#include "Physics4D.h"

Player::Player() : shape{}
{
	Vector4 directions[4]{ Vector4{ 1, 0, 0, 0 }, Vector4{ 0, 1, 0, 0 }, Vector4{ 0, 0, 1, 0 }, Vector4{ 0, 0, 0, 1 } };
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				for (int w = -1; w <= 1; w++) {
					if (x != 0 || y != 0 || z != 0 || w != 0) {
						shape.push_back(Vector4{ (FPN)x, (FPN)y, (FPN)z, (FPN)w });
					}
				}
			}
		}
	}
	for (Vector4& vector : shape) {
		vector.Normalize();
	}
}
Vector4 Player::GetScreenCenter() const
{
	return cameraZoom * (forward * (FPN)cos((double)pitch) + Vector4(0, 0, 0, (FPN)sin((double)pitch)));
}
Vector4 Player::GetScreenXVector() const
{
	return Vector4(forward.Y * screenUp.Z - forward.Z * screenUp.Y, forward.Z * screenUp.X - forward.X * screenUp.Z, forward.X * screenUp.Y - forward.Y * screenUp.X);
}
Vector4 Player::GetScreenYVector() const
{
	return -screenUp;
}
void Player::HandleViewControlls(FPN forwardInputHorizontal, FPN forwardInputVertical, FPN pitchInput)
{
	forward += forwardInputVertical * screenUp + forwardInputHorizontal * GetScreenXVector();
	forward.Normalize();
	screenUp -= forwardInputVertical * forward;
	screenUp -= forward * screenUp * forward;
	screenUp.Normalize();

	pitch += pitchInput;
	constexpr FPN maxPitch = (FPN)((FPN)2 / (FPN)5 * (FPN)constants::pi);
	if (pitch > maxPitch)
	{
		pitch = maxPitch;
	}
	if (pitch < -maxPitch)
	{
		pitch = -maxPitch;
	}
}
