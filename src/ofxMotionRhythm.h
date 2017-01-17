#include <ofMain.h>

class ofxMotionRhythm
{
public:
	ofxMotionRhythm();
	~ofxMotionRhythm();

	void computeRhythm(vector<float> WeightEffort);
	void draw(int x, int y, int w, int h);
	void keyPressed(int key);

private:
	int mStep;
	int mOffset;
	int mTempoX;
	int mTempoY;
};