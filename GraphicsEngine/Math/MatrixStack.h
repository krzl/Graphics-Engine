#pragma once

#include <stack>

#include "Matrix.h"

class MatrixStack
{
private:

	Matrix4f                    currentMatrix;
	std::stack<Matrix4f>        stack;

public:

	MatrixStack() : 
		currentMatrix() 
	{}

	Matrix4f&                   Top();
	void                        SetMatrix(Matrix4f& matrix);
	void                        Concatenate(Matrix4f& matrix);
	void                        PreConcatenate(Matrix4f& matrix);
	void                        Push();
	void                        Pop();
};

class PushStack
{
private:

	MatrixStack&                matrixStack;

public:

	PushStack(MatrixStack& matrixStack);
	~PushStack();
};