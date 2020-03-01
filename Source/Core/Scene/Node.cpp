#include "Node.h"
EFFECTS_NAMESPACE_BEGIN

void Node::setParent(std::shared_ptr<Node> newParent)
{
	std::shared_ptr<Node> oldParent = mParent;
	if (oldParent == newParent)
		return;

	std::shared_ptr<Node> curNode = newParent;
	while (curNode)
	{
		if (curNode == shared_from_this())
			return;
		curNode = curNode->getParent();
	}

	if (oldParent)
	{
		for (std::vector<std::shared_ptr<Node>>::iterator iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); )
		{
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == shared_from_this())
				oldParent->mChildren.erase(iter);
		}
	}

	mParent = newParent;
	if (newParent)
	{
		newParent->mChildren.push_back(shared_from_this());
	}
}

void Node::appendChildren(std::shared_ptr<Node> newChildren) 
{
	if (!newChildren)
		return;

	std::shared_ptr<Node> curNode = std::dynamic_pointer_cast<Node>(shared_from_this());
	while (curNode)
	{
		if (curNode == newChildren)
			return;
		curNode = curNode->getParent();
	}

	std::shared_ptr<Node> oldParent = newChildren->mParent;
	if (oldParent)
	{
		for (std::vector<std::shared_ptr<Node>>::iterator iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); )
		{
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == newChildren)
				oldParent->mChildren.erase(iter);
		}
	}
	
	newChildren->mParent = shared_from_this();
	mChildren.push_back(newChildren);
}

void Node::removeChildren(std::shared_ptr<Node> Children)
{
	for (std::vector<std::shared_ptr<Node>>::iterator iter = mChildren.begin(); iter != mChildren.end(); )
	{
		if (iter == mChildren.end())
			break;
		if (*iter == Children)
		{
			mChildren.erase(iter);
			Children->mParent = NULL;
		}
	}
}

void Node::setTransform(const glm::vec3 &pos, const glm::vec3 &scale, const glm::quat &rot)
{
    mLPos = pos;
    mLScale = scale;
    mLRot = rot;
}

glm::mat4 Node::getLocalMatrix()
{
	//RTS
	glm::mat4 R, T, S;
	R = glm::toMat4(mLRot);
	T = glm::translate(glm::mat4(1.0),mLPos);
	S = glm::scale(glm::mat4(1.0),mLScale);
	return R*T*S;
}

glm::mat4 Node::getWorldMatrix()
{
	glm::mat4 out = getLocalMatrix();
	std::shared_ptr<Node> cur = mParent;
	while (cur)
	{
		out = cur->getLocalMatrix() * out;
	}
	return out;
}

glm::vec3 Node::getRightVector()
{
    return getAxisX(getWorldMatrix());
}

glm::vec3 Node::getUpVector()
{
    return getAxisY(getWorldMatrix());
}

glm::vec3 Node::getFrontVector()
{
    return getAxisZ(getWorldMatrix());
}

EFFECTS_NAMESPACE_END