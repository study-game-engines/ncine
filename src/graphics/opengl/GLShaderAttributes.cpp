#include "GLShaderAttributes.h"
#include "GLShaderProgram.h"
#include "HashMapIterator.h"

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

StaticArray<GLShaderAttributes::GLVertexAttribPointerState, GLShaderAttributes::MaxDefinedVertexAttribPointers> GLShaderAttributes::definedPointers_;

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

GLShaderAttributes::GLShaderAttributes()
	: shaderProgram_(NULL), vertexAttributes_(VertexAttributesHashSize)
{
}

GLShaderAttributes::GLShaderAttributes(GLShaderProgram *shaderProgram)
	: shaderProgram_(NULL), vertexAttributes_(VertexAttributesHashSize)
{
	setProgram(shaderProgram);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void GLShaderAttributes::setProgram(GLShaderProgram *shaderProgram)
{
	shaderProgram_ = shaderProgram;
	vertexAttributes_.clear();
	importAttributes();
}

GLVertexAttribute* GLShaderAttributes::attribute(const char *name)
{
	GLVertexAttribute* vertexAttribute = NULL;

	if (shaderProgram_)
	{
		vertexAttribute = vertexAttributes_.find(name);

		if (vertexAttribute == NULL)
		{
			LOGW_X("Attribute \"%s\" not found in shader program %u", name, shaderProgram_->glHandle());
		}
	}
	else
	{
		LOGE_X("Cannot find attribute \"%s\", no shader program associated", name);
	}

	return vertexAttribute;
}

void GLShaderAttributes::defineVertexPointers(GLuint buondVboHandle)
{
	if (shaderProgram_)
	{
		for (StringHashMapIterator<GLVertexAttribute>::type i = i.begin(vertexAttributes_); i != i.end(vertexAttributes_); ++i)
		{
			GLVertexAttribute& attribute = (*i).value;
			int location = attribute.shaderAttribute()->location();
			if (definedPointers_[location] != attribute || definedPointers_[location].boundVbo() != buondVboHandle)
			{
				(*i).value.vertexAttribPointer();
				definedPointers_[location] = attribute;
				definedPointers_[location].setBoundVbo(buondVboHandle);
			}
			if (definedPointers_[location].isEnabled() == false)
			{
				(*i).value.enable();
				definedPointers_[location].setEnabled(true);
			}
		}
	}
	else
	{
		LOGE("No shader program associated");
	}
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void GLShaderAttributes::importAttributes()
{
	int count = shaderProgram_->attributes_.size();
	if (count > VertexAttributesHashSize)
	{
		LOGW_X("More active attributes (%d) than hashmap buckets (%d)", count, VertexAttributesHashSize);
	}

	for (int i = 0; i < count; i++)
	{
		const GLAttribute& attribute = shaderProgram_->attributes_[i];
		GLVertexAttribute vertexAttribute(&attribute);

		vertexAttributes_[attribute.name()] = vertexAttribute;
	}
}

GLShaderAttributes::GLVertexAttribPointerState::GLVertexAttribPointerState()
	: enabled_(false), size_(-1), type_(GL_FLOAT), vboStride_(0), vboPointer_(NULL)
{

}

bool GLShaderAttributes::GLVertexAttribPointerState::operator==(const GLVertexAttribute& attribute) const
{
	if (attribute.shaderAttribute()->numComponents() == size_ &&
		attribute.shaderAttribute()->basicType() == type_ &&
		attribute.vboStride() == vboStride_ && attribute.vboPointer() == vboPointer_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GLShaderAttributes::GLVertexAttribPointerState::operator!=(const GLVertexAttribute& attribute) const
{
	return !operator==(attribute);
}

GLShaderAttributes::GLVertexAttribPointerState& GLShaderAttributes::GLVertexAttribPointerState::operator=(const GLVertexAttribute& attribute)
{
	size_ = attribute.shaderAttribute()->numComponents();
	type_ = attribute.shaderAttribute()->basicType();
	vboStride_ = attribute.vboStride();
	vboPointer_ = attribute.vboPointer();

	return *this;
}

}
