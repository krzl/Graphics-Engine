#include "MatrixStack.h"

Matrix4f& MatrixStack::Top()
{
	return currentMatrix;
}

void MatrixStack::Concatenate(Matrix4f& otherM)
{
	currentMatrix = currentMatrix * otherM;
}

void MatrixStack::PreConcatenate(Matrix4f& otherM)
{
	currentMatrix = otherM * currentMatrix;
}

void MatrixStack::Push()
{
	stack.push(currentMatrix);
}

void MatrixStack::Pop()
{
	currentMatrix = stack.top();
	stack.pop();
}

void MatrixStack::SetMatrix(Matrix4f& matrix)
{
	currentMatrix = matrix;
}

PushStack::PushStack(MatrixStack& stack) 
	: matrixStack(stack)
{
	matrixStack.Push();
}

PushStack::~PushStack()
{
	matrixStack.Pop();
}