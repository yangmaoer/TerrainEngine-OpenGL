#include "Water.h"

Water::Water(glm::vec2 position, Shader* shad, float scale, float height, unsigned int dudvMap, unsigned int normalMap, Model * waterPlane) : shad(shad), dudvMap(dudvMap), normalMap(normalMap), waterPlane(waterPlane) {
	glm::mat4 identity;
	glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
	glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position.x, height, position.y));
	this->modelMatrix = transMatrix * scaleMatrix;

	reflectionFBO = new FrameBufferObject(FBOw, FBOh);
	refractionFBO = new FrameBufferObject(FBOw, FBOh);
}

void Water::bindReflectionFBO() {
	reflectionFBO->bind();
}

void Water::bindRefractionFBO() {
	refractionFBO->bind();
}
void Water::draw(glm::mat4 gVP, glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPosition) {
	// draw water plane
	shad->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shad->setMat4("modelMatrix", modelMatrix);
	shad->setMat4("gVP", gVP);

	shad->setVec3("u_LightColor", lightColor);
	shad->setVec3("u_LightPosition", lightPosition);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionFBO->tex);
	shad->setInt("reflectionTex", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionFBO->tex);
	shad->setInt("refractionTex", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
	shad->setInt("waterDUDV", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	shad->setInt("normalMap", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, refractionFBO->depthTex);
	shad->setInt("depthMap", 4);

	float waveSpeed = 0.25;
	float time = glfwGetTime();

	float moveFactor = waveSpeed * time;
	shad->setFloat("moveFactor", moveFactor);

	shad->setVec3("cameraPosition", viewPosition);

	waterPlane->Draw(*shad);
	glDisable(GL_BLEND);
}

void Water::unbindFBO() {
	unbindCurrentFrameBuffer(Window::SCR_WIDTH, Window::SCR_WIDTH);
}



Water::~Water()
{
}
