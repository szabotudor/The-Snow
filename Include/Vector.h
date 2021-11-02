#pragma once

namespace ss {
	class Vector {
	public:
		float x = 0.0f, y = 0.0f;

		Vector(float x, float y);
		Vector(float w = 0);

		//Overrides for the vector class operators
		Vector operator + (float x);
		Vector operator - (float x);
		Vector operator * (float x);
		Vector operator / (float x);
		Vector operator += (float x);
		Vector operator -= (float x);
		Vector operator *= (float x);
		Vector operator /= (float x);
		Vector operator + (Vector v);
		Vector operator - (Vector v);
		Vector operator * (Vector v);
		Vector operator / (Vector v);
		Vector operator = (float x);

		//Returns the normalized verctor without changing the vector that is being normalized
		Vector normalized();
		//Normalizes the vector
		void normalize();
		//Returns the legnth of the vector
		float lenght();
	};
}
