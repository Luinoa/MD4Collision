#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#define uint unsigned int
#define HEX8 setw(8) << setfill('0') << hex

using namespace std;

uint F(uint p, uint q, uint r) {
	return (p & q) | (~p & r);
}

uint G(uint p, uint q, uint r) {
	return (p & q) | (p & r) | (q & r);
}

uint H(uint p, uint q, uint r) {
	return p ^ q ^ r;
}

uint atPosi(uint x, int posi) {
	return (x >> (posi - 1)) & 1;
}

uint circleLeft(uint x, int n) {
	return (x << n) | (x >> (32 - n));
}

uint circleRight(uint x, int n) {
	return (x >> n) | (x << (32 - n));
}

int main() {
	vector<uint> M(16), M_(16);
	random_device seed;
	ranlux48 engine(seed());
	uniform_int_distribution<uint> distrib(0, 0xFFFFFFFF);

	uint a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;

	while (true) {
		for (auto& word : M) {
			word = distrib(engine);
		}

		// Stage 1
		// Add a1[14] = b0[14].
		uint a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
		a1 = a1 ^ ((atPosi(a1, 7) ^ atPosi(b0, 7)) << 6) ^ ((atPosi(a1, 14) ^ atPosi(b0, 14)) << 13);

		M[0] = circleRight(a1, 3) - a0 - F(b0, c0, d0);
		a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
		M_[0] = M[0];
		uint a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
		cout << "M0:" << HEX8 << M[0] << " M0':" << HEX8 << M_[0] << " a1:" << HEX8 << a1 << " a1':" << HEX8 << a1_ << endl;

		// Stage 2
		// Add d1[14] = 0.
		uint d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
		d1 = d1 ^ (atPosi(d1, 7) << 6) ^ ((atPosi(d1, 8) ^ atPosi(a1, 8)) << 7) ^ ((atPosi(d1, 11) ^ atPosi(a1, 11)) << 10) ^ (atPosi(d1, 14) << 13);
		M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
		d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
		M_[1] = M[1] + 0x80000000;
		uint d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
		cout << "M1:" << HEX8 << M[1] << " M1':" << HEX8 << M_[1] << " d1:" << HEX8 << d1 << " d1':" << HEX8 << d1_ << endl;

		// Stage 3
		// Add c1[14] = 0, c1[19] = 1.
		uint c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
		c1 = (c1 | (1 << 6) | (1 << 7) | (1 << 18)) ^ (atPosi(c1, 11) << 10) ^ (atPosi(c1, 14) << 13) ^ ((atPosi(c1, 26) ^ atPosi(d1, 26)) << 25);
		M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
		c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
		M_[2] = M[2] + 0x70000000;
		uint c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
		cout << "M2:" << HEX8 << M[2] << " M2':" << HEX8 << M_[2] << " c1:" << HEX8 << c1 << " c1':" << HEX8 << c1_ << endl;

		// Stage 4
		// Add b1[19] = c1[19].
		uint b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
		b1 = (b1 | (1 << 6)) ^ (atPosi(b1, 8) << 7) ^ (atPosi(b1, 11) << 10) ^ (atPosi(b1, 14) << 13) ^ ((atPosi(b1, 19) ^ atPosi(c1, 19)) << 18) ^
			(atPosi(b1, 26) << 25);
		M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
		b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
		M_[3] = M[3];
		uint b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
		cout << "M3:" << HEX8 << M[3] << " M3':" << HEX8 << M_[3] << " b1:" << HEX8 << b1 << " b1':" << HEX8 << b1_ << endl;

		// Stage 5
		// Add a2[17, 18, 23] = b1[17, 18, 23], a2[19] = 0.
		uint a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
		a2 = (a2 | (1 << 7) | (1 << 10)) ^ (atPosi(a2, 19) << 18) ^ (atPosi(a2, 26) << 25) ^ ((atPosi(a2, 14) ^ atPosi(b1, 14)) << 13) ^
			((atPosi(a2, 17) ^ atPosi(b1, 17)) << 16) ^ ((atPosi(a2, 18) ^ atPosi(b1, 18)) << 17) ^ ((atPosi(a2, 23) ^ atPosi(b1, 23)) << 22);
		M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
		a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
		M_[4] = M[4];
		uint a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
		cout << "M4:" << HEX8 << M[4] << " M4':" << HEX8 << M_[4] << " a2:" << HEX8 << a2 << " a2':" << HEX8 << a2_ << endl;

		// Stage 6
		// Add d2[17, 18, 23] = 0.
		uint d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
		d2 = (d2 | (1 << 25)) ^ (atPosi(d2, 14) << 13) ^ (atPosi(d2, 17) << 16) ^ (atPosi(d2, 18) << 17) ^ ((atPosi(d2, 19) ^ atPosi(a2, 19)) << 18) ^
			((atPosi(d2, 20) ^ atPosi(a2, 20)) << 19) ^ ((atPosi(d2, 21) ^ atPosi(a2, 21)) << 20) ^ ((atPosi(d2, 22) ^ atPosi(a2, 22)) << 21) ^
			(atPosi(d2, 23) << 22);
		M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
		d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
		M_[5] = M[5];
		uint d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
		cout << "M5:" << HEX8 << M[5] << " M5':" << HEX8 << M_[5] << " d2:" << HEX8 << d2 << " d2':" << HEX8 << d2_ << endl;

		// Stage 7
		// Add c2[17, 18, 23] = 0.
		uint c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
		c2 = (c2 | (1 << 20)) ^ ((atPosi(c2, 13) ^ atPosi(d2, 13)) << 12) ^ (atPosi(c2, 14) << 13) ^ ((atPosi(c2, 15) ^ atPosi(d2, 15)) << 14) ^
			(atPosi(c2, 17) << 16) ^ (atPosi(c2, 18) << 17) ^ (atPosi(c2, 19) << 18) ^ (atPosi(c2, 20) << 19) ^ (atPosi(c2, 22) << 21) ^
			(atPosi(c2, 23) << 22);
		M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
		c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
		M_[6] = M[6];
		uint c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
		cout << "M6:" << HEX8 << M[6] << " M6':" << HEX8 << M_[6] << " c2:" << HEX8 << c2 << " c2':" << HEX8 << c2_ << endl;

		// Stage 8
		// Add b2[18, 23] = 0.
		uint b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
		b2 = (b2 | (1 << 12) | (1 << 13)) ^ (atPosi(b2, 15) << 14) ^ ((atPosi(b2, 17) ^ atPosi(c2, 17)) << 16) ^ (atPosi(b2, 18) << 17) ^
			(atPosi(b2, 19) << 18) ^ (atPosi(b2, 20) << 19) ^ (atPosi(b2, 21) << 20) ^ (atPosi(b2, 22) << 21) ^ (atPosi(b2, 23) << 22);
		M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
		b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
		M_[7] = M[7];
		uint b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
		cout << "M7:" << HEX8 << M[7] << " M7':" << HEX8 << M_[7] << " b2:" << HEX8 << b2 << " b2':" << HEX8 << b2_ << endl;

		// Stage 9
		// Add a3[16] = b2[16].
		uint a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
		a3 = (a3 | (1 << 12) | (1 << 13) | (1 << 14) | (1 << 21)) ^ ((atPosi(a3, 16) ^ atPosi(b2, 16)) << 15) ^ (atPosi(a3, 17) << 16) ^
			(atPosi(a3, 19) << 18) ^ (atPosi(a3, 20) << 19) ^ (atPosi(a3, 21) << 20) ^ ((atPosi(a3, 23) ^ atPosi(b2, 23)) << 22) ^
			((atPosi(a3, 26) ^ atPosi(b2, 26)) << 25);
		M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
		a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
		M_[8] = M[8];
		uint a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
		cout << "M8:" << HEX8 << M[8] << " M8':" << HEX8 << M_[8] << " a3:" << HEX8 << a3 << " a3':" << HEX8 << a3_ << endl;

		// Stage 10
		// Add d3[16, 19] = 0.
		uint d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
		d3 = (d3 | (1 << 12) | (1 << 13) | (1 << 14) | (1 << 20) | (1 << 21) | (1 << 25)) ^ (atPosi(d3, 16) << 15) ^ (atPosi(d3, 17) << 16) ^
			(atPosi(d3, 19) << 18) ^ (atPosi(d3, 20) << 19) ^ (atPosi(d3, 23) << 22) ^ ((atPosi(d3, 30) ^ atPosi(a3, 30)) << 29);
		M[9] = circleRight(d3, 7) - d2 - F(a3, b2, c2);
		d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
		M_[9] = M[9];
		uint d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
		cout << "M9:" << HEX8 << M[9] << " M9':" << HEX8 << M_[9] << " d3:" << HEX8 << d3 << " d3':" << HEX8 << d3_ << endl;

		// Stage 11
		// Add c3[16, 19] = 0.
		uint c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
		c3 = (c3 | (1 << 16) | (1 << 29)) ^ (atPosi(c3, 16) << 15) ^ (atPosi(c3, 19) << 18) ^ (atPosi(c3, 20) << 19) ^ (atPosi(c3, 21) << 20) ^
			(atPosi(c3, 22) << 21) ^ (atPosi(c3, 23) << 22) ^ (atPosi(c3, 26) << 25) ^ ((atPosi(c3, 32) ^ atPosi(d3, 32)) << 31);
		M[10] = circleRight(c3, 11) - c2 - F(d3, a3, b2);
		c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
		M_[10] = M[10];
		uint c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
		cout << "M10:" << HEX8 << M[10] << " M10':" << HEX8 << M_[10] << " c3:" << HEX8 << c3 << " c3':" << HEX8 << c3_ << endl;

		// Stage 12
		// Add b3[16, 17, 19] = 0.
		uint b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
		b3 = (b3 | (1 << 20) | (1 << 21) | (1 << 25)) ^ (atPosi(b3, 16) << 15) ^ (atPosi(b3, 17) << 16) ^ (atPosi(b3, 19) << 18) ^ (atPosi(b3, 20) << 19) ^
			((atPosi(b3, 23) ^ atPosi(c3, 23)) << 22) ^ (atPosi(b3, 30) << 29) ^ (atPosi(b3, 32) << 31);
		M[11] = circleRight(b3, 19) - b2 - F(c3, d3, a3);
		b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
		M_[11] = M[11];
		uint b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
		cout << "M11:" << HEX8 << M[11] << " M11':" << HEX8 << M_[11] << " b3:" << HEX8 << b3 << " b3':" << HEX8 << b3_ << endl;

		// Stage 13
		// Add a4[17] = 0.
		uint a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
		a4 = (a4 | (1 << 29)) ^ (atPosi(a4, 17) << 16) ^ (atPosi(a4, 23) << 22) ^ (atPosi(a4, 26) << 25) ^ ((atPosi(a4, 27) ^ atPosi(b3, 27)) << 26)
			^ ((atPosi(a4, 29) ^ atPosi(b3, 29)) << 28) ^ (atPosi(a4, 32) << 31);
		M[12] = circleRight(a4, 3) - a3 - F(b3, c3, d3);
		a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
		M_[12] = M[12] - 0x00010000;
		uint a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
		cout << "M12:" << HEX8 << M[12] << " M12':" << HEX8 << M_[12] << " a4:" << HEX8 << a4 << " a4':" << HEX8 << a4_ << endl;

		// Stage 14
		// Add d4[17] = 1.
		uint d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
		d4 = (d4 | (1 << 16) | (1 << 26) | (1 << 28) | (1 << 31)) ^ (atPosi(d4, 23) << 22) ^ (atPosi(d4, 26) << 25) ^ (atPosi(d4, 30) << 29);
		M[13] = circleRight(d4, 7) - d3 - F(a4, b3, c3);
		d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
		M_[13] = M[13];
		uint d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
		cout << "M13:" << HEX8 << M[13] << " M13':" << HEX8 << M_[13] << " d4:" << HEX8 << d4 << " d4':" << HEX8 << d4_ << endl;

		// Stage 15
		uint c4 = circleLeft(c3 + F(d4, a4, b3) + M[14], 11);
		c4 = (c4 | (1 << 22) | (1 << 25)) ^ ((atPosi(c4, 19) ^ atPosi(d4, 19)) << 18) ^ (atPosi(c4, 27) << 26) ^ (atPosi(c4, 29) << 28)
			^ (atPosi(c4, 30) << 29);
		M[14] = circleRight(c4, 11) - c3 - F(d4, a4, b3);
		c4 = circleLeft(c3 + F(d4, a4, b3) + M[14], 11);
		M_[14] = M[14];
		uint c4_ = circleLeft(c3_ + F(d4_, a4_, b3_) + M_[14], 11);
		cout << "M14:" << HEX8 << M[14] << " M14':" << HEX8 << M_[14] << " c4:" << HEX8 << c4 << " c4':" << HEX8 << c4_ << endl;

		// Stage 16
		// WRONG: Add b4[32] = c4[32].
		uint b4 = circleLeft(b3 + F(c4, d4, a4) + M[15], 19);
		b4 = (b4 | (1 << 26) | (1 << 28)) ^ (atPosi(b4, 19) << 18) ^ ((atPosi(b4, 26) ^ atPosi(c4, 26)) << 25) ^ (atPosi(b4, 30) << 29) ^
			((atPosi(b4, 32) ^ atPosi(c4, 32)) << 31);
		M[15] = circleRight(b4, 19) - b3 - F(c4, d4, a4);
		b4 = circleLeft(b3 + F(c4, d4, a4) + M[15], 19);
		M_[15] = M[15];
		uint b4_ = circleLeft(b3_ + F(c4_, d4_, a4_) + M_[15], 19);
		cout << "M15:" << HEX8 << M[15] << " M15':" << HEX8 << M_[15] << " b4:" << HEX8 << b4 << " b4':" << HEX8 << b4_ << endl;

		// Stage 17
		uint a5, a5_;
		while (true) {
			a5 = circleLeft(a4 + G(b4, c4, d4) + M[0] + 0x5A827999, 3);
			a5_ = circleLeft(a4_ + G(b4_, c4_, d4_) + M_[0] + 0x5A827999, 3);
			cout << "\nM0:" << HEX8 << M[0] << " M0':" << HEX8 << M_[0] << " a5:" << HEX8 << a5 << " a5':" << HEX8 << a5_ << endl;

			if (atPosi(a5, 19) != atPosi(c4, 19)) {
				cout << "Adjusting a5[19]:" << endl;
				M[0] += atPosi(a1, 19) ? -(1 << 15) : (1 << 15);
				a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
				M_[0] = M[0];
				a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
				cout << "Adjust M0 to: " << HEX8 << M[0] << " Adjust M0' to: " << HEX8 << M_[0] << endl;
				cout << "Adjusted a1: " << HEX8 << a1 << " Adjusted a1': " << HEX8 << a1_ << endl;
				M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7) << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				continue;
			}

			if (atPosi(a5, 26) != 1) {
				cout << "Adjusting a5[26]:" << endl;
				M[0] += atPosi(a1, 26) ? -(1 << 22) : (1 << 22);
				a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
				M_[0] = M[0];
				a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
				cout << "Adjust M0 to: " << HEX8 << M[0] << " Adjust M0' to: " << HEX8 << M_[0] << endl;
				cout << "Adjusted a1: " << HEX8 << a1 << " Adjusted a1': " << HEX8 << a1_ << endl;
				M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7) << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				continue;
			}

			if (atPosi(a5, 27) != 0) {
				cout << "Adjusting a5[27]:" << endl;
				M[0] += atPosi(a1, 27) ? -(1 << 23) : (1 << 23);
				a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
				M_[0] = M[0];
				a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
				cout << "Adjust M0 to: " << HEX8 << M[0] << " Adjust M0' to: " << HEX8 << M_[0] << endl;
				cout << "Adjusted a1: " << HEX8 << a1 << " Adjusted a1': " << HEX8 << a1_ << endl;
				M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7) << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				continue;
			}

			if (atPosi(a5, 29) != 1) {
				cout << "Adjusting a5[29]:" << endl;
				M[0] += atPosi(a1, 29) ? -(1 << 25) : (1 << 25);
				a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
				M_[0] = M[0];
				a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
				cout << "Adjust M0 to: " << HEX8 << M[0] << " Adjust M0' to: " << HEX8 << M_[0] << endl;
				cout << "Adjusted a1: " << HEX8 << a1 << " Adjusted a1': " << HEX8 << a1_ << endl;
				M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7) << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				continue;
			}

			if (atPosi(a5, 32) != 1) {
				cout << "Adjusting a5[32]:" << endl;
				M[0] += atPosi(a1, 32) ? -(1 << 28) : (1 << 28);
				a1 = circleLeft(a0 + F(b0, c0, d0) + M[0], 3);
				M_[0] = M[0];
				a1_ = circleLeft(a0 + F(b0, c0, d0) + M_[0], 3);
				cout << "Adjust M0 to: " << HEX8 << M[0] << " Adjust M0' to: " << HEX8 << M_[0] << endl;
				cout << "Adjusted a1: " << HEX8 << a1 << " Adjusted a1': " << HEX8 << a1_ << endl;
				M[1] = circleRight(d1, 7) - d0 - F(a1, b0, c0);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7) << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				continue;
			}

			break;
		}

		// Stage 18
		uint d5, d5_;
		while (true) {
			d5 = circleLeft(d4 + G(a5, b4, c4) + M[4] + 0x5A827999, 5);
			M_[4] = M[4];
			d5_ = circleLeft(d4_ + G(a5_, b4_, c4_) + M_[4] + 0x5A827999, 5);
			cout << "\nM4:" << HEX8 << M[4] << " M4':" << HEX8 << M_[4] << " d5:" << HEX8 << d5 << " d5':" << HEX8 << d5_ << endl;

			if (atPosi(d5, 19) != atPosi(a5, 19)) {
				cout << "Adjusting d5[19]:" << endl;
				M[1] += 1 << 6;
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << d1_ << endl;
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				M[4] -= 1 << 13;
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] -= 1 << 13;
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				continue;
			}

			if (atPosi(d5, 26) != atPosi(b4, 26)) {
				cout << "Adjusting d5[26]:" << endl;
				M[4] += atPosi(a2, 24) ? -(1 << 20) : (1 << 20);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				M_[6] = M[6];
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjust M6 to: " << HEX8 << M[6] << " Adjust M6' to: " << HEX8 << M_[6] << endl;
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				continue;
			}

			if (atPosi(d5, 27) != atPosi(b4, 27)) {
				cout << "Adjusting d5[27]:" << endl;
				M[4] += atPosi(a2, 25) ? -(1 << 21) : (1 << 21);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				M_[6] = M[6];
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjust M6 to: " << HEX8 << M[6] << " Adjust M6' to: " << HEX8 << M_[6] << endl;
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				continue;
			}

			if (atPosi(d5, 29) != atPosi(b4, 29)) {
				cout << "Adjusting d5[29]:" << endl;
				M[4] += atPosi(a2, 27) ? -(1 << 23) : (1 << 23);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				M_[6] = M[6];
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjust M6 to: " << HEX8 << M[6] << " Adjust M6' to: " << HEX8 << M_[6] << endl;
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				continue;
			}

			if (atPosi(d5, 32) != atPosi(b4, 32)) {
				cout << "Adjusting d5[32]:" << endl;
				M[4] += atPosi(a2, 30) ? -(1 << 26) : (1 << 26);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				M_[6] = M[6];
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjust M6 to: " << HEX8 << M[6] << " Adjust M6' to: " << HEX8 << M_[6] << endl;
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				continue;
			}

			break;
		}

		// Stage 19
		uint c5, c5_;
		while (true) {
			c5 = circleLeft(c4 + G(d5, a5, b4) + M[8] + 0x5A827999, 9);
			M_[8] = M[8];
			c5_ = circleLeft(c4_ + G(d5_, a5_, b4_) + M_[8] + 0x5A827999, 9);
			cout << "\nM8:" << HEX8 << M[8] << " M8':" << HEX8 << M_[8] << " c5:" << HEX8 << c5 << " c5':" << HEX8 << c5_ << endl;

			if (atPosi(c5, 26) != atPosi(d5, 26)) {
				cout << "Adjusting c5[26]:" << endl;
				M[5] += 1 << 9;
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] -= 1 << 16;
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] -= 1 << 16;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				continue;
			}

			if (atPosi(c5, 27) != atPosi(d5, 27)) {
				cout << "Adjusting c5[27]:" << endl;
				M[5] += 1 << 10;
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] -= 1 << 17;
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] -= 1 << 17;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				continue;
			}

			if (atPosi(c5, 29) != atPosi(d5, 29)) {
				cout << "Adjusting c5[29]:" << endl;
				M[4] += 1 << 15;
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] += 1 << 11;
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] -= 1 << 18;
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] -= 1 << 19;
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] -= 1 << 18;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				continue;
			}

			if (atPosi(c5, 30) != atPosi(d5, 30)) {
				cout << "Adjusting c5[30]:" << endl;
				M[8] += atPosi(a3, 24) ? -(1 << 20) : (1 << 20);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] = circleRight(d3, 7) - d2 - F(a3, b2, c2);
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				M[10] = circleRight(c3, 11) - c2 - F(d3, a3, b2);
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				M_[10] = M[10];
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjust M10 to: " << HEX8 << M[10] << " Adjust M10' to: " << HEX8 << M_[10] << endl;
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				M[11] = circleRight(b3, 19) - b2 - F(c3, d3, a3);
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				M_[11] = M[11];
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjust M11 to: " << HEX8 << M[11] << " Adjust M11' to: " << HEX8 << M_[11] << endl;
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] = circleRight(a4, 3) - a3 - F(b3, c3, d3);
				a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				continue;
			}

			if (atPosi(c5, 32) != atPosi(d5, 32)) {
				cout << "Adjusting c5[32]:" << endl;
				M[5] += 1 << 15;
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] -= 1 << 22;
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] -= 1 << 22;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				continue;
			}

			break;
		}

		// Stage 20
		uint b5, b5_;
		while (true) {
			b5 = circleLeft(b4 + G(c5, d5, a5) + M[12] + 0x5A827999, 13);
			M_[12] = M[12] - 0x00010000;
			b5_ = circleLeft(b4_ + G(c5_, d5_, a5_) + M_[12] + 0x5A827999, 13);
			cout << "\nM12:" << HEX8 << M[12] << " M12':" << HEX8 << M_[12] << " b5:" << HEX8 << b5 << " b5':" << HEX8 << b5_ << endl;

			if (atPosi(b5, 29) != atPosi(c5, 29)) {
				cout << "Adjusting b5[29]:" << endl;
				M[9] += 1 << 8;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] -= 1 << 15;
				a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				M[13] -= 1 << 15;
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				M_[13] = M[13];
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjust M13 to: " << HEX8 << M[13] << " Adjust M13' to: " << HEX8 << M_[13] << endl;
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				continue;
			}

			if (atPosi(b5, 30) != 1) {
				cout << "Adjusting b5[30]:" << endl;
				M[11] += 1 << 29;
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				M_[11] = M[11];
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjust M11 to: " << HEX8 << M[11] << " Adjust M11' to: " << HEX8 << M_[11] << endl;
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] -= 1 << 16;
				a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				c4 = circleLeft(c3 + F(d4, a4, b3) + M[14], 11);
				c4_ = circleLeft(c3_ + F(d4_, a4_, b3_) + M_[14], 11);
				cout << "Adjusted c4: " << HEX8 << c4 << " Adjusted c4': " << HEX8 << c4_ << endl;
				M[15] -= 1 << 16;
				b4 = circleLeft(b3 + F(c4, d4, a4) + M[15], 19);
				M_[15] = M[15];
				b4_ = circleLeft(b3_ + F(c4_, d4_, a4_) + M_[15], 19);
				cout << "Adjust M15 to: " << HEX8 << M[15] << " Adjust M15' to: " << HEX8 << M_[15] << endl;
				cout << "Adjusted b4: " << HEX8 << b4 << " Adjusted b4': " << HEX8 << b4_ << endl;
				continue;
			}

			if (atPosi(b5, 32) != 0) {
				cout << "Adjusting b5[32]:" << endl;
				M[9] += 1 << 11;
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] -= 1 << 18;
				a4 = circleLeft(a3 + F(b3, c3, d3) + M[12], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				M[13] -= 1 << 18;
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				M_[13] = M[13];
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjust M13 to: " << HEX8 << M[13] << " Adjust M13' to: " << HEX8 << M_[13] << endl;
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				continue;
			}

			break;
		}

		// Stage 21
		uint a6, a6_;
		while (true) {
			a6 = circleLeft(a5 + G(b5, c5, d5) + M[1] + 0x5A827999, 3);
			M_[1] = M[1] + 0x80000000;
			a6_ = circleLeft(a5_ + G(b5_, c5_, d5_) + M_[1] + 0x5A827999, 3);
			cout << "\nM1:" << HEX8 << M[1] << " M1':" << HEX8 << M_[1] << " a6:" << HEX8 << a6 << " a6':" << HEX8 << a6_ << endl;

			if (atPosi(a6, 29) != 1) {
				cout << "Adjusting a6[29]:" << endl;
				M[1] += atPosi(d1, 1) ? -(1 << 25) : (1 << 25);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << d1_ << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				continue;
			}

			// WRONG: Add a6[30] = 0.
			if (atPosi(a6, 30) != 0) {
				cout << "Adjusting a6[30]:" << endl;
				M[1] += atPosi(d1, 2) ? -(1 << 26) : (1 << 26);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << d1_ << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				continue;
			}

			if (atPosi(a6, 32) != 1) {
				cout << "Adjusting a6[32]:" << endl;
				M[1] += atPosi(d1, 4) ? -(1 << 28) : (1 << 28);
				d1 = circleLeft(d0 + F(a1, b0, c0) + M[1], 7);
				M_[1] = M[1] + 0x80000000;
				d1_ = circleLeft(d0 + F(a1_, b0, c0) + M_[1], 7);
				cout << "Adjust M1 to: " << HEX8 << M[1] << " Adjust M1' to: " << HEX8 << M_[1] << endl;
				cout << "Adjusted d1: " << HEX8 << d1 << " Adjusted d1': " << HEX8 << d1_ << endl;
				M[2] = circleRight(c1, 11) - c0 - F(d1, a1, b0);
				c1 = circleLeft(c0 + F(d1, a1, b0) + M[2], 11);
				M_[2] = M[2] + 0x70000000;
				c1_ = circleLeft(c0 + F(d1_, a1_, b0) + M_[2], 11);
				cout << "Adjust M2 to: " << HEX8 << M[2] << " Adjust M2' to: " << HEX8 << M_[2] << endl;
				cout << "Adjusted c1: " << HEX8 << c1 << " Adjusted c1': " << HEX8 << c1_ << endl;
				M[3] = circleRight(b1, 19) - b0 - F(c1, d1, a1);
				b1 = circleLeft(b0 + F(c1, d1, a1) + M[3], 19);
				M_[3] = M[3];
				b1_ = circleLeft(b0 + F(c1_, d1_, a1_) + M_[3], 19);
				cout << "Adjust M3 to: " << HEX8 << M[3] << " Adjust M3' to: " << HEX8 << M_[3] << endl;
				cout << "Adjusted b1: " << HEX8 << b1 << " Adjusted b1': " << HEX8 << b1_ << endl;
				M[4] = circleRight(a2, 3) - a1 - F(b1, c1, d1);
				a2 = circleLeft(a1 + F(b1, c1, d1) + M[4], 3);
				M_[4] = M[4];
				a2_ = circleLeft(a1_ + F(b1_, c1_, d1_) + M_[4], 3);
				cout << "Adjust M4 to: " << HEX8 << M[4] << " Adjust M4' to: " << HEX8 << M_[4] << endl;
				cout << "Adjusted a2: " << HEX8 << a2 << " Adjusted a2': " << HEX8 << a2_ << endl;
				M[5] = circleRight(d2, 7) - d1 - F(a2, b1, c1);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				continue;
			}

			break;
		}

		// Stage 22
		uint d6, d6_;
		while (true) {
			d6 = circleLeft(d5 + G(a6, b5, c5) + M[5] + 0x5A827999, 5);
			M_[5] = M[5];
			d6_ = circleLeft(d5_ + G(a6_, b5_, c5_) + M_[5] + 0x5A827999, 5);
			cout << "\nM5:" << HEX8 << M[5] << " M5':" << HEX8 << M_[5] << " d6:" << HEX8 << d6 << " d6':" << HEX8 << d6_ << endl;

			if (atPosi(d6, 29) != atPosi(b5, 29)) {
				cout << "Adjusting d6[29]:" << endl;
				M[5] += atPosi(d2, 31) ? -(1 << 23) : (1 << 23);
				d2 = circleLeft(d1 + F(a2, b1, c1) + M[5], 7);
				M_[5] = M[5];
				d2_ = circleLeft(d1_ + F(a2_, b1_, c1_) + M_[5], 7);
				cout << "Adjust M5 to: " << HEX8 << M[5] << " Adjust M5' to: " << HEX8 << M_[5] << endl;
				cout << "Adjusted d2: " << HEX8 << d2 << " Adjusted d2': " << HEX8 << d2_ << endl;
				M[6] = circleRight(c2, 11) - c1 - F(d2, a2, b1);
				c2 = circleLeft(c1 + F(d2, a2, b1) + M[6], 11);
				M_[6] = M[6];
				c2_ = circleLeft(c1_ + F(d2_, a2_, b1_) + M_[6], 11);
				cout << "Adjust M6 to: " << HEX8 << M[6] << " Adjust M6' to: " << HEX8 << M_[6] << endl;
				cout << "Adjusted c2: " << HEX8 << c2 << " Adjusted c2': " << HEX8 << c2_ << endl;
				M[7] = circleRight(b2, 19) - b1 - F(c2, d2, a2);
				b2 = circleLeft(b1 + F(c2, d2, a2) + M[7], 19);
				M_[7] = M[7];
				b2_ = circleLeft(b1_ + F(c2_, d2_, a2_) + M_[7], 19);
				cout << "Adjust M7 to: " << HEX8 << M[7] << " Adjust M7' to: " << HEX8 << M_[7] << endl;
				cout << "Adjusted b2: " << HEX8 << b2 << " Adjusted b2': " << HEX8 << b2_ << endl;
				M[8] = circleRight(a3, 3) - a2 - F(b2, c2, d2);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] = circleRight(d3, 7) - d2 - F(a3, b2, c2);
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				continue;
			}

			break;
		}

		// Stage 23
		uint c6, c6_;
		while (true) {
			c6 = circleLeft(c5 + G(d6, a6, b5) + M[9] + 0x5A827999, 9);
			M_[9] = M[9];
			c6_ = circleLeft(c5_ + G(d6_, a6_, b5_) + M_[9] + 0x5A827999, 9);
			cout << "\nM9:" << HEX8 << M[9] << " M9':" << HEX8 << M_[9] << " c6:" << HEX8 << c6 << " c6':" << HEX8 << c6_ << endl;

			if (atPosi(c6, 29) != atPosi(d6, 29)) {
				cout << "Adjusting c6[29]:" << endl;
				M[9] += atPosi(d3, 27) ? -(1 << 19) : (1 << 19);
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				M[10] = circleRight(c3, 11) - c2 - F(d3, a3, b2);
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				M_[10] = M[10];
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjust M10 to: " << HEX8 << M[10] << " Adjust M10' to: " << HEX8 << M_[10] << endl;
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				M[11] = circleRight(b3, 19) - b2 - F(c3, d3, a3);
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				M_[11] = M[11];
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjust M11 to: " << HEX8 << M[11] << " Adjust M11' to: " << HEX8 << M_[11] << endl;
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] = circleRight(a4, 3) - a3 - F(b3, c3, d3);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				M[13] = circleRight(d4, 7) - d3 - F(a4, b3, c3);
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				M_[13] = M[13];
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjust M13 to: " << HEX8 << M[13] << " Adjust M13' to: " << HEX8 << M_[13] << endl;
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				continue;
			}

			if (atPosi(c6, 30) == atPosi(d6, 30)) {
				cout << "Adjusting c6[30]:" << endl;
				M[9] += atPosi(d3, 28) ? -(1 << 20) : (1 << 20);
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				M[10] = circleRight(c3, 11) - c2 - F(d3, a3, b2);
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				M_[10] = M[10];
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjust M10 to: " << HEX8 << M[10] << " Adjust M10' to: " << HEX8 << M_[10] << endl;
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				M[11] = circleRight(b3, 19) - b2 - F(c3, d3, a3);
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				M_[11] = M[11];
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjust M11 to: " << HEX8 << M[11] << " Adjust M11' to: " << HEX8 << M_[11] << endl;
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] = circleRight(a4, 3) - a3 - F(b3, c3, d3);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				M[13] = circleRight(d4, 7) - d3 - F(a4, b3, c3);
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				M_[13] = M[13];
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjust M13 to: " << HEX8 << M[13] << " Adjust M13' to: " << HEX8 << M_[13] << endl;
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				continue;
			}

			if (atPosi(c6, 32) == atPosi(d6, 32)) {
				cout << "Adjusting c6[32]:" << endl;
				M[8] += atPosi(a3, 30) ? -(1 << 26) : (1 << 26);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[8] = M[8];
				a3_ = circleLeft(a2_ + F(b2_, c2_, d2_) + M_[8], 3);
				cout << "Adjust M8 to: " << HEX8 << M[8] << " Adjust M8' to: " << HEX8 << M_[8] << endl;
				cout << "Adjusted a3: " << HEX8 << a3 << " Adjusted a3': " << HEX8 << a3_ << endl;
				M[9] = circleRight(d3, 7) - d2 - F(a3, b2, c2);
				M[9] += atPosi(d3, 30) ? -(1 << 22) : (1 << 22);
				d3 = circleLeft(d2 + F(a3, b2, c2) + M[9], 7);
				M_[9] = M[9];
				d3_ = circleLeft(d2_ + F(a3_, b2_, c2_) + M_[9], 7);
				cout << "Adjust M9 to: " << HEX8 << M[9] << " Adjust M9' to: " << HEX8 << M_[9] << endl;
				cout << "Adjusted d3: " << HEX8 << d3 << " Adjusted d3': " << HEX8 << d3_ << endl;
				M[10] = circleRight(c3, 11) - c2 - F(d3, a3, b2);
				c3 = circleLeft(c2 + F(d3, a3, b2) + M[10], 11);
				M_[10] = M[10];
				c3_ = circleLeft(c2_ + F(d3_, a3_, b2_) + M_[10], 11);
				cout << "Adjust M10 to: " << HEX8 << M[10] << " Adjust M10' to: " << HEX8 << M_[10] << endl;
				cout << "Adjusted c3: " << HEX8 << c3 << " Adjusted c3': " << HEX8 << c3_ << endl;
				M[11] = circleRight(b3, 19) - b2 - F(c3, d3, a3);
				b3 = circleLeft(b2 + F(c3, d3, a3) + M[11], 19);
				M_[11] = M[11];
				b3_ = circleLeft(b2_ + F(c3_, d3_, a3_) + M_[11], 19);
				cout << "Adjust M11 to: " << HEX8 << M[11] << " Adjust M11' to: " << HEX8 << M_[11] << endl;
				cout << "Adjusted b3: " << HEX8 << b3 << " Adjusted b3': " << HEX8 << b3_ << endl;
				M[12] = circleRight(a4, 3) - a3 - F(b3, c3, d3);
				a3 = circleLeft(a2 + F(b2, c2, d2) + M[8], 3);
				M_[12] = M[12] - 0x00010000;
				a4_ = circleLeft(a3_ + F(b3_, c3_, d3_) + M_[12], 3);
				cout << "Adjust M12 to: " << HEX8 << M[12] << " Adjust M12' to: " << HEX8 << M_[12] << endl;
				cout << "Adjusted a4: " << HEX8 << a4 << " Adjusted a4': " << HEX8 << a4_ << endl;
				M[13] = circleRight(d4, 7) - d3 - F(a4, b3, c3);
				d4 = circleLeft(d3 + F(a4, b3, c3) + M[13], 7);
				M_[13] = M[13];
				d4_ = circleLeft(d3_ + F(a4_, b3_, c3_) + M_[13], 7);
				cout << "Adjust M13 to: " << HEX8 << M[13] << " Adjust M13' to: " << HEX8 << M_[13] << endl;
				cout << "Adjusted d4: " << HEX8 << d4 << " Adjusted d4': " << HEX8 << d4_ << endl;
				continue;
			}

			break;
		}

		// Stage 24
		cout << "\n";
		uint b6 = circleLeft(b5 + G(c6, d6, a6) + M[13] + 0x5A827999, 13);
		uint b6_ = circleLeft(b5_ + G(c6_, d6_, a6_) + M_[13] + 0x5A827999, 13);
		cout << "b6:" << HEX8 << b6 << " b6':" << HEX8 << b6_ << endl;

		// Stage 25
		uint a7 = circleLeft(a6 + G(b6, c6, d6) + M[2] + 0x5A827999, 3);
		uint a7_ = circleLeft(a6_ + G(b6_, c6_, d6_) + M_[2] + 0x5A827999, 3);
		cout << "a7:" << HEX8 << a7 << " a7':" << HEX8 << a7_ << endl;

		// Stage 26
		uint d7 = circleLeft(d6 + G(a7, b6, c6) + M[6] + 0x5A827999, 5);
		uint d7_ = circleLeft(d6_ + G(a7_, b6_, c6_) + M_[6] + 0x5A827999, 5);
		cout << "d7:" << HEX8 << d7 << " d7':" << HEX8 << d7_ << endl;

		// Stage 27
		uint c7 = circleLeft(c6 + G(d7, a7, b6) + M[10] + 0x5A827999, 9);
		uint c7_ = circleLeft(c6_ + G(d7_, a7_, b6_) + M_[10] + 0x5A827999, 9);
		cout << "c7:" << HEX8 << c7 << " c7':" << HEX8 << c7_ << endl;

		// Stage 28
		uint b7 = circleLeft(b6 + G(c7, d7, a7) + M[14] + 0x5A827999, 13);
		uint b7_ = circleLeft(b6_ + G(c7_, d7_, a7_) + M_[14] + 0x5A827999, 13);
		cout << "b7:" << HEX8 << b7 << " b7':" << HEX8 << b7_ << endl;

		// Stage 29
		uint a8 = circleLeft(a7 + G(b7, c7, d7) + M[3] + 0x5A827999, 3);
		uint a8_ = circleLeft(a7_ + G(b7_, c7_, d7_) + M_[3] + 0x5A827999, 3);
		cout << "a8:" << HEX8 << a8 << " a8':" << HEX8 << a8_ << endl;

		// Stage 30
		uint d8 = circleLeft(d7 + G(a8, b7, c7) + M[7] + 0x5A827999, 5);
		uint d8_ = circleLeft(d7_ + G(a8_, b7_, c7_) + M_[7] + 0x5A827999, 5);
		cout << "d8:" << HEX8 << d8 << " d8':" << HEX8 << d8_ << endl;

		// Stage 31
		uint c8 = circleLeft(c7 + G(d8, a8, b7) + M[11] + 0x5A827999, 9);
		uint c8_ = circleLeft(c7_ + G(d8_, a8_, b7_) + M_[11] + 0x5A827999, 9);
		cout << "c8:" << HEX8 << c8 << " c8':" << HEX8 << c8_ << endl;

		// Stage 32
		uint b8 = circleLeft(b7 + G(c8, d8, a8) + M[15] + 0x5A827999, 13);
		uint b8_ = circleLeft(b7_ + G(c8_, d8_, a8_) + M_[15] + 0x5A827999, 13);
		cout << "b8:" << HEX8 << b8 << " b8':" << HEX8 << b8_ << endl;

		// Stage 33
		uint a9 = circleLeft(a8 + H(b8, c8, d8) + M[0] + 0x6ED9EBA1, 3);
		uint a9_ = circleLeft(a8_ + H(b8_, c8_, d8_) + M_[0] + 0x6ED9EBA1, 3);
		cout << "a9:" << HEX8 << a9 << " a9':" << HEX8 << a9_ << endl;

		// Stage 34
		uint d9 = circleLeft(d8 + H(a9, b8, c8) + M[8] + 0x6ED9EBA1, 9);
		uint d9_ = circleLeft(d8_ + H(a9_, b8_, c8_) + M_[8] + 0x6ED9EBA1, 9);
		cout << "d9:" << HEX8 << d9 << " d9':" << HEX8 << d9_ << endl;

		// Stage 35
		uint c9 = circleLeft(c8 + H(d9, a9, b8) + M[4] + 0x6ED9EBA1, 11);
		uint c9_ = circleLeft(c8_ + H(d9_, a9_, b8_) + M_[4] + 0x6ED9EBA1, 11);
		cout << "c9:" << HEX8 << c9 << " c9':" << HEX8 << c9_ << endl;

		// Stage 36
		uint b9 = circleLeft(b8 + H(c9, d9, a9) + M[12] + 0x6ED9EBA1, 15);
		uint b9_ = circleLeft(b8_ + H(c9_, d9_, a9_) + M_[12] + 0x6ED9EBA1, 15);
		cout << "b9:" << HEX8 << b9 << " b9':" << HEX8 << b9_ << endl;
		if (atPosi(b9, 32) != 1) {
			cout << "Failed on checking b9[32] = 1.\n" << endl;
			continue;
		}

		// Stage 37
		uint a10 = circleLeft(a9 + H(b9, c9, d9) + M[2] + 0x6ED9EBA1, 3);
		uint a10_ = circleLeft(a9_ + H(b9_, c9_, d9_) + M_[2] + 0x6ED9EBA1, 3);
		cout << "a10:" << HEX8 << a10 << " a10':" << HEX8 << a10_ << endl;
		if (atPosi(a10, 32) != 1) {
			cout << "Failed on checking a10[32] = 1.\n" << endl;
			continue;
		}

		// Stage 38
		uint d10 = circleLeft(d9 + H(a10, b9, c9) + M[10] + 0x6ED9EBA1, 9);
		uint d10_ = circleLeft(d9_ + H(a10_, b9_, c9_) + M_[10] + 0x6ED9EBA1, 9);
		cout << "d10:" << HEX8 << d10 << " d10':" << HEX8 << d10_ << endl;

		// Stage 39
		uint c10 = circleLeft(c9 + H(d10, a10, b9) + M[6] + 0x6ED9EBA1, 11);
		uint c10_ = circleLeft(c9_ + H(d10_, a10_, b9_) + M_[6] + 0x6ED9EBA1, 11);
		cout << "c10:" << HEX8 << c10 << " c10':" << HEX8 << c10_ << endl;

		// Stage 40
		uint b10 = circleLeft(b9 + H(c10, d10, a10) + M[14] + 0x6ED9EBA1, 15);
		uint b10_ = circleLeft(b9_ + H(c10_, d10_, a10_) + M_[14] + 0x6ED9EBA1, 15);
		cout << "b10:" << HEX8 << b10 << " b10':" << HEX8 << b10_ << endl;

		// Stage 41
		uint a11 = circleLeft(a10 + H(b10, c10, d10) + M[1] + 0x6ED9EBA1, 3);
		uint a11_ = circleLeft(a10_ + H(b10_, c10_, d10_) + M_[1] + 0x6ED9EBA1, 3);
		cout << "a11:" << HEX8 << a11 << " a11':" << HEX8 << a11_ << endl;

		// Stage 42
		uint d11 = circleLeft(d10 + H(a11, b10, c10) + M[9] + 0x6ED9EBA1, 9);
		uint d11_ = circleLeft(d10_ + H(a11_, b10_, c10_) + M_[9] + 0x6ED9EBA1, 9);
		cout << "d11:" << HEX8 << d11 << " d11':" << HEX8 << d11_ << endl;

		// Stage 43
		uint c11 = circleLeft(c10 + H(d11, a11, b10) + M[5] + 0x6ED9EBA1, 11);
		uint c11_ = circleLeft(c10_ + H(d11_, a11_, b10_) + M_[5] + 0x6ED9EBA1, 11);
		cout << "c11:" << HEX8 << c11 << " c11':" << HEX8 << c11_ << endl;

		// Stage 44
		uint b11 = circleLeft(b10 + H(c11, d11, a11) + M[13] + 0x6ED9EBA1, 15);
		uint b11_ = circleLeft(b10_ + H(c11_, d11_, a11_) + M_[13] + 0x6ED9EBA1, 15);
		cout << "b11:" << HEX8 << b11 << " b11':" << HEX8 << b11_ << endl;

		// Stage 45
		uint a12 = circleLeft(a11 + H(b11, c11, d11) + M[3] + 0x6ED9EBA1, 3);
		uint a12_ = circleLeft(a11_ + H(b11_, c11_, d11_) + M_[3] + 0x6ED9EBA1, 3);
		cout << "a12:" << HEX8 << a12 << " a12':" << HEX8 << a12_ << endl;

		// Stage 46
		uint d12 = circleLeft(d11 + H(a12, b11, c11) + M[11] + 0x6ED9EBA1, 9);
		uint d12_ = circleLeft(d11_ + H(a12_, b11_, c11_) + M_[11] + 0x6ED9EBA1, 9);
		cout << "d12:" << HEX8 << d12 << " d12':" << HEX8 << d12_ << endl;

		// Stage 47
		uint c12 = circleLeft(c11 + H(d12, a12, b11) + M[7] + 0x6ED9EBA1, 11);
		uint c12_ = circleLeft(c11_ + H(d12_, a12_, b11_) + M_[7] + 0x6ED9EBA1, 11);
		cout << "c12:" << HEX8 << c12 << " c12':" << HEX8 << c12_ << endl;

		// Stage 48
		uint b12 = circleLeft(b11 + H(c12, d12, a12) + M[15] + 0x6ED9EBA1, 15);
		uint b12_ = circleLeft(b11_ + H(c12_, d12_, a12_) + M_[15] + 0x6ED9EBA1, 15);
		cout << "b12:" << HEX8 << b12 << " b12':" << HEX8 << b12_ << endl;

		break;
	}

	cout << "\nFound a collision!" << endl;
	cout << "M:" << endl;
	for (int i = 15; i >= 8; i--) {
		cout << HEX8 << M[i] << " ";
	}
	cout << endl;
	for (int i = 7; i >= 0; i--) {
		cout << HEX8 << M[i] << " ";
	}

	cout << "\nM':" << endl;
	for (int i = 15; i >= 8; i--) {
		cout << HEX8 << M_[i] << " ";
	}
	cout << endl;
	for (int i = 7; i >= 0; i--) {
		cout << HEX8 << M_[i] << " ";
	}
}
