/*
 * Config.h
 *
 *  Created on: 28 May 2014
 *      Author: erik
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <boost/program_options.hpp>
#include <string>
#include <opencv/cv.h>

namespace util {

using std::string;

namespace po = boost::program_options;

class Config {
public:
	static bool loadConfig(std::string);


	// getters
	//com serial
	/*
	static bool getComPortId(){
		return comPort_id;
	}
	*/

	static bool isCalibrationEnabled() {
		return calibration_enabled;
	}

	static int getCameraHeight() {
		return camera_height;
	}

	static int getCameraWidth() {
		return camera_width;
	}

	static const string& getNetworkAddress() {
		return network_address;
	}

	static bool isNetworkEnabled() {
		return network_enabled;
	}

	static int getNetworkPort() {
		return network_port;
	}

	static const cv::Scalar& getBlueHsVmax() {
		return blueHSVmax;
	}

	static const cv::Scalar& getBlueHsVmin() {
		return blueHSVmin;
	}

	static int getCameraDeviceId() {
		return camera_deviceId;
	}

	static const cv::Scalar& getGreenHsVmax() {
		return greenHSVmax;
	}

	static const cv::Scalar& getGreenHsVmin() {
		return greenHSVmin;
	}

	static const cv::Scalar& getRedHsVmax() {
		return redHSVmax;
	}

	static const cv::Scalar& getRedHsVmin() {
		return redHSVmin;
	}

	static int getVirtualFeedHeight() {
		return virtualFeed_height;
	}

	static int getVirtualFeedWidth() {
		return virtualFeed_width;
	}

	static int getNumRobots() {
		return numRobots;
	}

	static int getFrameDelay() {
		return frameDelay;
	}

	static int getCameraBrightness() {
		return camera_brightness;
	}

	static int getCameraContrast() {
		return camera_contrast;
	}

	static int getCameraSaturation() {
		return camera_saturation;
	}

	static float getCameraLedSize() {
		return camera_ledSize;
	}

	static int getCameraMaxNumObjects() {
		return camera_maxNumObjects;
	}

	static int getCameraMinObjectArea() {
		return camera_minObjectArea;
	}

	static int getEvictionLimit() {
		return evictionLimit;
	}

	static int getHistoryLength() {
		return historyLength;
	}

	static float getRobotDiameter() {
		return robotDiameter;
	}

	static float getRobotRotation() {
		return robotRotationalSpeed;
	}

	static float getRobotSpeed() {
		return robotSpeed;
	}

	static char getControlLoadConfig() {
		return control_loadConfig;
	}

	static float getRobotRotationalSpeed() {
		return robotRotationalSpeed;
	}

private:
	static void addOptions(po::options_description &desc);
	static void parseOptions(po::variables_map &vm);
	//com serial
	// com port
	//static int comPort_id;


	// camera
	static int camera_deviceId;
	static int camera_width;
	static int camera_height;
	static int camera_brightness;
	static int camera_contrast;
	static int camera_saturation;
	static int camera_maxNumObjects;
	static int camera_minObjectArea;
	static float camera_ledSize;

	// colors
	static cv::Scalar redHSVmin;
	static cv::Scalar redHSVmax;
	static cv::Scalar blueHSVmin;
	static cv::Scalar blueHSVmax;
	static cv::Scalar greenHSVmin;
	static cv::Scalar greenHSVmax;

	// virtualFeed
	static int virtualFeed_width;
	static int virtualFeed_height;

	// network
	static bool network_enabled;
	static string network_address;
	static int network_port;

	// general
	static bool calibration_enabled;
	static int frameDelay;

	// tracking
	static int numRobots;
	static int evictionLimit;
	static float robotSpeed;
	static float robotRotationalSpeed;
	static float robotDiameter;
	static int historyLength;

	// controls
	static char control_loadConfig;

};

} /* namespace util */

#endif /* CONFIG_H_ */
