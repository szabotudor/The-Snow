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
		Vector operator + (Vector v);
		Vector operator - (Vector v);
		Vector operator * (Vector v);
		Vector operator / (Vector v);

		void operator += (float x);
		void operator -= (float x);
		void operator *= (float x);
		void operator /= (float x);
		void operator += (Vector x);
		void operator -= (Vector x);
		void operator *= (Vector x);
		void operator /= (Vector x);

		Vector operator = (float x);

		//Returns the normalized verctor without changing the vector that is being normalized
		Vector normalized();
		//Normalizes the vector
		void normalize();
		//Returns the legnth of the vector
		float lenght();
	};

	class iVector {
	public:
		int x = 0, y = 0;

		iVector(int x, int y);
		iVector(int w = 0);

		//Overrides for the vector class operators
		iVector operator + (int x);
		iVector operator - (int x);
		iVector operator * (int x);
		iVector operator / (int x);
		iVector operator += (int x);
		iVector operator -= (int x);
		iVector operator *= (int x);
		iVector operator /= (int x);
		iVector operator + (iVector v);
		iVector operator - (iVector v);
		iVector operator * (iVector v);
		iVector operator / (iVector v);
		iVector operator = (int x);

		//Returns the legnth of the vector
		float lenght();
	};
}
