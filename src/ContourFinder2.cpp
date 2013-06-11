#include "ContourFinder2.h"
#include "ofxCv/Wrappers.h"

namespace ofxCv {
	
	using namespace cv;
	
	ContourFinder2::ContourFinder2()
	:autoThreshold(true)
	,invert(false)
	,simplify(true)
	,thresholdValue(128.)
    ,blurValue( 0 )
    ,dilateValue( 0 )
	,useTargetColor(false)
	,contourFindingMode(CV_RETR_EXTERNAL){
		resetMinArea();
		resetMaxArea();
	}
	
	void ContourFinder2::findContours(Mat img) {
		// threshold the image using a tracked color or just binary grayscale
		if(useTargetColor) {
			Scalar offset(thresholdValue, thresholdValue, thresholdValue);
			Scalar base = toCv(targetColor);
			if(trackingColorMode == TRACK_COLOR_RGB) {
				inRange(img, base - offset, base + offset, thresh);
			} else {
				if(TRACK_COLOR_H) {
					offset[1] = 255;
					offset[2] = 255;
				}
				if(TRACK_COLOR_HS) {
					offset[2] = 255;
				}
				cvtColor(img, hsvBuffer, CV_RGB2HSV);
				base = toCv(convertColor(targetColor, CV_RGB2HSV));
				Scalar lowerb = base - offset;
				Scalar upperb = base + offset;
				inRange(hsvBuffer, lowerb, upperb, thresh);
			}
		} else {
			if(img.channels() == 1) {
				thresh = img.clone();
			} else if(img.channels() == 3) {
				cvtColor(img, thresh, CV_RGB2GRAY);
			} else if(img.channels() == 4) {
				cvtColor(img, thresh, CV_RGBA2GRAY);
			}
		}
		if(autoThreshold) {
			threshold(thresh, thresholdValue, invert);
		}
        
        cv::Mat dilateKernel( cv::Size( dilateValue, dilateValue), CV_8UC1, cv::Scalar(1) );
        
        blur( thresh, blurValue );
        dilate( thresh, thresh, dilateKernel );
		
		// run the contour finder
		vector<vector<cv::Point> > allContours;
		int simplifyMode = simplify ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE;
		cv::findContours(thresh, allContours, contourFindingMode, simplifyMode);
		
		// filter the contours
		bool needMinFilter = (minArea > 0);
		bool needMaxFilter = maxAreaNorm ? (maxArea < 1) : (maxArea < numeric_limits<float>::infinity());
		if(needMinFilter || needMaxFilter) {
			contours.clear();
			double imgArea = img.rows * img.cols;
			double imgMinArea = minAreaNorm ? (minArea * imgArea) : minArea;
			double imgMaxArea = maxAreaNorm ? (maxArea * imgArea) : maxArea;
			for(int i = 0; i < (int)allContours.size(); i++) {
				double curArea = contourArea(Mat(allContours[i]));
				if((!needMinFilter || curArea >= imgMinArea) &&
					 (!needMaxFilter || curArea <= imgMaxArea)) {
					contours.push_back(allContours[i]);
				}
			}
		} else {
			contours = allContours;
		}
		
		// generate polylines from the contours
		polylines.clear();
		for(int i = 0; i < (int)size(); i++) {
			polylines.push_back(toOf(contours[i]));
		}
		
		// generate bounding boxes from the contours
		boundingRects.clear();
		for(int i = 0; i < (int)size(); i++) {
			boundingRects.push_back(boundingRect(Mat(contours[i])));
		}
		
		// track bounding boxes
		tracker.track(boundingRects);
	}
	

	void ContourFinder2::setFindHoles(bool findHoles){
		if(findHoles){
			contourFindingMode = CV_RETR_LIST;
		}else{
			contourFindingMode = CV_RETR_EXTERNAL;
		}
	}

	const vector<vector<cv::Point> >& ContourFinder2::getContours() const {
		return contours;
	}
	
	const vector<ofPolyline>& ContourFinder2::getPolylines() const {
		return polylines;
	}
	
	const vector<cv::Rect>& ContourFinder2::getBoundingRects() const {
		return boundingRects;
	}
	
	unsigned int ContourFinder2::size() const {
		return contours.size();
	}
	
	vector<cv::Point>& ContourFinder2::getContour(unsigned int i) {
		return contours[i];
	}
	
	ofPolyline& ContourFinder2::getPolyline(unsigned int i) {
		return polylines[i];
	}
	
	cv::Rect ContourFinder2::getBoundingRect(unsigned int i) const {
		return boundingRects[i];
	}
	
	cv::Point2f ContourFinder2::getCenter(unsigned int i) const {
		cv::Rect box = getBoundingRect(i);
		return cv::Point2f(box.x + box.width / 2, box.y + box.height / 2);
	}
	
	cv::Point2f ContourFinder2::getCentroid(unsigned int i) const {
		Moments m = moments(Mat(contours[i]));
		return cv::Point2f(m.m10 / m.m00, m.m01 / m.m00);
	}
	
	cv::Point2f ContourFinder2::getAverage(unsigned int i) const {
		Scalar average = mean(Mat(contours[i]));
		return cv::Point2f(average[0], average[1]);
	}
	
	cv::Vec2f ContourFinder2::getBalance(unsigned int i) const {
		return cv::Vec2f(getCentroid(i) - getCenter(i));
	}
	
	double ContourFinder2::getContourArea(unsigned int i) const {
		return contourArea(Mat(contours[i]));
	}
	
	double ContourFinder2::getArcLength(unsigned int i) const {
		return arcLength(Mat(contours[i]), true);
	}
	
	vector<cv::Point> ContourFinder2::getConvexHull(unsigned int i) const {
		vector<cv::Point> hull;
		convexHull(Mat(contours[i]), hull);
		return hull;
	}
	
	vector<cv::Vec4i> ContourFinder2::getConvexityDefects(unsigned int i) const {
		return convexityDefects(contours[i]);
	}
	
	cv::RotatedRect ContourFinder2::getMinAreaRect(unsigned int i) const {
		return minAreaRect(Mat(contours[i]));
	}
	
	cv::Point2f ContourFinder2::getMinEnclosingCircle(unsigned int i, float& radius) const {
		cv::Point2f center;
		minEnclosingCircle(Mat(contours[i]), center, radius);
		return center;
	}
	
	cv::RotatedRect ContourFinder2::getFitEllipse(unsigned int i) const {
		if(contours[i].size() < 5) {
			return getMinAreaRect(i);
		}
		return fitEllipse(Mat(contours[i]));
	}
	
	vector<cv::Point> ContourFinder2::getFitQuad(unsigned int i) const {
		vector<cv::Point> convexHull = getConvexHull(i);		
		vector<cv::Point> quad = convexHull;
		
		static const unsigned int targetPoints = 4;
		static const unsigned int maxIterations = 16;
		static const double infinity = numeric_limits<double>::infinity();
		double minEpsilon = 0;
		double maxEpsilon = infinity;
		double curEpsilon = 16; // good initial guess
		
		// unbounded binary search to simplify the convex hull until it's 4 points
		if(quad.size() > 4) {
			for(int i = 0; i <(int) maxIterations; i++) {
				approxPolyDP(Mat(convexHull), quad, curEpsilon, true);
				if(quad.size() == targetPoints) {
					break;
				}
				if(quad.size() > targetPoints) {
					minEpsilon = curEpsilon;
					if(maxEpsilon == infinity) {
						curEpsilon = curEpsilon *  2;
					} else {
						curEpsilon = (maxEpsilon + minEpsilon) / 2;
					}
				}
				if(quad.size() < targetPoints) {
					maxEpsilon = curEpsilon;
					curEpsilon = (maxEpsilon + minEpsilon) / 2;
				}
			}
		}
		
		return quad;
	}
	
	cv::Vec2f ContourFinder2::getVelocity(unsigned int i) const {
		return tracker.getVelocity(i);
	}
	
	unsigned int ContourFinder2::getLabel(unsigned int i) const {
		return tracker.getCurrentLabels()[i];
	}
	
	RectTracker& ContourFinder2::getTracker() {
		return tracker;
	}
	
	void ContourFinder2::setAutoThreshold(bool autoThreshold) {
		this->autoThreshold = autoThreshold;
	}
	
	void ContourFinder2::setThreshold(float thresholdValue) {
		this->thresholdValue = thresholdValue;
	}
	
    void ContourFinder2::setBlur( int blurValue_ )
    {
        blurValue       = blurValue_;
    };
    
    void ContourFinder2::setDilate( int dilateValue_ )
    {
        dilateValue     = dilateValue_;
    };
    
	void ContourFinder2::setInvert(bool invert) {
		this->invert = invert;
	}
	
	void ContourFinder2::setTargetColor(ofColor targetColor, TrackingColorMode trackingColorMode) {
		useTargetColor = true;
		this->targetColor = targetColor;
		this->trackingColorMode = trackingColorMode;
	}
	
	void ContourFinder2::setSimplify(bool simplify) {
		this->simplify = simplify;
	}
	
	void ContourFinder2::draw() {
		ofPushStyle();
		ofNoFill();
		for(int i = 0; i < (int)polylines.size(); i++) {
			polylines[i].draw();
			ofRect(toOf(getBoundingRect(i)));
		}
		ofPopStyle();
	}
	
	void ContourFinder2::resetMinArea() {
		setMinArea(0);
	}
	
	void ContourFinder2::resetMaxArea() {
		setMaxArea(numeric_limits<float>::infinity());
	}
	
	void ContourFinder2::setMinArea(float minArea) {
		this->minArea = minArea;
		minAreaNorm = false;
	}
	
	void ContourFinder2::setMaxArea(float maxArea) {
		this->maxArea = maxArea;
		maxAreaNorm = false;
	}
	
	void ContourFinder2::setMinAreaRadius(float minAreaRadius) {
		minArea = PI * minAreaRadius * minAreaRadius;
		minAreaNorm = false;
	}
	
	void ContourFinder2::setMaxAreaRadius(float maxAreaRadius) {
		maxArea = PI * maxAreaRadius * maxAreaRadius;
		maxAreaNorm = false;
	}
	
	void ContourFinder2::setMinAreaNorm(float minAreaNorm) {
		minArea = minAreaNorm;
		this->minAreaNorm = true;
	}
	
	void ContourFinder2::setMaxAreaNorm(float maxAreaNorm) {
		maxArea = maxAreaNorm;
		this->maxAreaNorm = true;
	}
	
}
