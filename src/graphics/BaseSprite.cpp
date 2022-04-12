#include "BaseSprite.h"
#include "RenderCommand.h"
#include "tracy.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

BaseSprite::BaseSprite(SceneNode *parent, Texture *texture, float xx, float yy)
    : DrawableNode(parent, xx, yy), texture_(texture), texRect_(0, 0, 0, 0),
      flippedX_(false), flippedY_(false), spriteBlock_(nullptr)
{
	renderCommand_->material().setBlendingEnabled(true);
}

BaseSprite::BaseSprite(SceneNode *parent, Texture *texture, const Vector2f &position)
    : BaseSprite(parent, texture, position.x, position.y)
{
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void BaseSprite::setSize(float width, float height)
{
	// Update anchor points when size changes
	if (anchorPoint_.x != 0.0f)
		anchorPoint_.x = (anchorPoint_.x / width_) * width;
	if (anchorPoint_.y != 0.0f)
		anchorPoint_.y = (anchorPoint_.y / height_) * height;

	width_ = width;
	height_ = height;
	dirtyBits_.set(DirtyBitPositions::SizeBit);
	dirtyBits_.set(DirtyBitPositions::AabbBit);
}

/*! \note If you set a texture that is already assigned, this method would be equivalent to `resetTexture()` */
void BaseSprite::setTexture(Texture *texture)
{
	// Allow self-assignment to take into account the case where the texture stays the same but it loads new data
	textureHasChanged(texture);
	texture_ = texture;
	dirtyBits_.set(DirtyBitPositions::TextureBit);
}

/*! \note Use this method when the content of the currently assigned texture changes */
void BaseSprite::resetTexture()
{
	textureHasChanged(texture_);
	dirtyBits_.set(DirtyBitPositions::TextureBit);
}

void BaseSprite::setTexRect(const Recti &rect)
{
	texRect_ = rect;
	setSize(static_cast<float>(rect.w), static_cast<float>(rect.h));

	if (flippedX_)
	{
		texRect_.x += texRect_.w;
		texRect_.w *= -1;
	}

	if (flippedY_)
	{
		texRect_.y += texRect_.h;
		texRect_.h *= -1;
	}

	dirtyBits_.set(DirtyBitPositions::TextureBit);
}

void BaseSprite::setFlippedX(bool flippedX)
{
	if (flippedX_ != flippedX)
	{
		texRect_.x += texRect_.w;
		texRect_.w *= -1;
		flippedX_ = flippedX;

		dirtyBits_.set(DirtyBitPositions::TextureBit);
	}
}

void BaseSprite::setFlippedY(bool flippedY)
{
	if (flippedY_ != flippedY)
	{
		texRect_.y += texRect_.h;
		texRect_.h *= -1;
		flippedY_ = flippedY;

		dirtyBits_.set(DirtyBitPositions::TextureBit);
	}
}

///////////////////////////////////////////////////////////
// PROTECTED FUNCTIONS
///////////////////////////////////////////////////////////

BaseSprite::BaseSprite(const BaseSprite &other)
    : DrawableNode(other), texture_(other.texture_), texRect_(other.texRect_),
      flippedX_(other.flippedX_), flippedY_(other.flippedY_), spriteBlock_(nullptr)
{
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void BaseSprite::updateRenderCommand()
{
	ZoneScoped;

	if (dirtyBits_.test(DirtyBitPositions::TransformationBit))
	{
		renderCommand_->setTransformation(worldMatrix_);
		dirtyBits_.reset(DirtyBitPositions::TransformationBit);
	}
	if (dirtyBits_.test(DirtyBitPositions::ColorBit))
	{
		spriteBlock_->uniform("color")->setFloatVector(Colorf(absColor()).data());
		dirtyBits_.reset(DirtyBitPositions::ColorBit);
	}
	if (dirtyBits_.test(DirtyBitPositions::SizeBit))
	{
		spriteBlock_->uniform("spriteSize")->setFloatValue(width_, height_);
		dirtyBits_.reset(DirtyBitPositions::SizeBit);
	}

	if (dirtyBits_.test(DirtyBitPositions::TextureBit))
	{
		if (texture_)
		{
			renderCommand_->material().setTexture(*texture_);

			const Vector2i texSize = texture_->size();
			const float texScaleX = texRect_.w / float(texSize.x);
			const float texBiasX = texRect_.x / float(texSize.x);
			const float texScaleY = texRect_.h / float(texSize.y);
			const float texBiasY = texRect_.y / float(texSize.y);

			spriteBlock_->uniform("texRect")->setFloatValue(texScaleX, texBiasX, texScaleY, texBiasY);
		}
		else
			renderCommand_->material().setTexture(nullptr);

		dirtyBits_.reset(DirtyBitPositions::TextureBit);
	}
}

}
