/*
 * Config.cpp
 *
 *  Created on: 28 May 2014
 *      Author: erik
 */

#include "Config.h"

#include <fstream>
#include <iostream>

namespace util {
//com serial
//comm port
//int Config::comPort_id = 0;

// camera
int Config::camera_deviceId = 0;
int Config::camera_width = 1280;
int Config::camera_height = 720;
double Config::camera_brightness = 0.1;//133;
int Config::camera_contrast = 5;
int Config::camera_saturation = 83;
int Config::camera_maxNumObjects = 1000;
int Config::camera_minObjectArea = 10;
float Config::camera_ledSize = 0.01;


// colors
cv::Scalar Config::redHSVmin;
cv::Scalar Config::redHSVmax;
cv::Scalar Config::blueHSVmin;
cv::Scalar Config::blueHSVmax;
cv::Scalar Config::greenHSVmin;
cv::Scalar Config::greenHSVmax;

// virtualFeed
int Config::virtualFeed_width = 500;
int Config::virtualFeed_height = 500;

// network
bool Config::network_enabled = true;
string Config::network_address = "localhost";
int Config::network_port = 52346;

// general
bool Config::calibration_enabled = false;
int Config::frameDelay = 5;

// tracking
int Config::numRobots = 8;
int Config::evictionLimit = 5;
float Config::robotSpeed = 0.02;
float Config::robotRotationalSpeed = 0.2;
float Config::robotDiameter = 0.75;
int Config::historyLength = 3;

// controls
char Config::control_loadConfig = 'l';


void Config::addOptions(po::options_description &desc) {
	//com serial
	//desc.add_options()("comPort.id", po::value<int>(&comPort_id));

	// camera
	desc.add_options()("camera.deviceId", po::value<int>(&camera_deviceId));
	desc.add_options()("camera.width", po::value<int>(&camera_width));
	desc.add_options()("camera.height", po::value<int>(&camera_height));
	desc.add_options()("camera.brightness", po::value<double>(&camera_brightness));
	desc.add_options()("camera.contrast", po::value<int>(&camera_contrast));
	desc.add_options()("camera.saturation", po::value<int>(&camera_saturation));
	desc.add_options()("camera.maxNumObjects", po::value<int>(&camera_maxNumObjects));
	desc.add_options()("camera.minObjectArea", po::value<int>(&camera_minObjectArea));
	desc.add_options()("camera.ledSize", po::value<float>(&camera_ledSize));

	// red
	desc.add_options()("red.hmin", po::value<int>());
	desc.add_options()("red.hmax", po::value<int>());
	desc.add_options()("red.smin", po::value<int>());
	desc.add_options()("red.smax", po::value<int>());
	desc.add_options()("red.vmin", po::value<int>());
	desc.add_options()("red.vmax", po::value<int>());
	// blue
	desc.add_options()("blue.hmin", po::value<int>());
	desc.add_options()("blue.hmax", po::value<int>());
	desc.add_options()("blue.smin", po::value<int>());
	desc.add_options()("blue.smax", po::value<int>());
	desc.add_options()("blue.vmin", po::value<int>());
	desc.add_options()("blue.vmax", po::value<int>());
	// green
	desc.add_options()("green.hmin", po::value<int>());
	desc.add_options()("green.hmax", po::value<int>());
	desc.add_options()("green.smin", po::value<int>());
	desc.add_options()("green.smax", po::value<int>());
	desc.add_options()("green.vmin", po::value<int>());
	desc.add_options()("green.vmax", po::value<int>());
	// virtualFeed
	desc.add_options()("virtualFeed.width", po::value<int>(&virtualFeed_width));
	desc.add_options()("virtualFeed.height", po::value<int>(&virtualFeed_height));
	// network
	desc.add_options()("network.enabled", po::value<bool>(&network_enabled));
	desc.add_options()("network.address", po::value<string>(&network_address));
	desc.add_options()("network.port", po::value<int>(&network_port));
	// general
	desc.add_options()("general.calibrationEnabled", po::value<bool>(&calibration_enabled));
	desc.add_options()("general.frameDelay", po::value<int>(&frameDelay));
	// tracking
	desc.add_options()("tracking.numRobots", po::value<int>(&numRobots));
	desc.add_options()("tracking.evictionLimit", po::value<int>(&evictionLimit));
	desc.add_options()("tracking.robotSpeed", po::value<float>(&robotSpeed));
	desc.add_options()("tracking.robotRotationalSpeed", po::value<float>(&robotRotationalSpeed));
	desc.add_options()("tracking.robotDiameter", po::value<float>(&robotDiameter));
	desc.add_options()("tracking.historyLength", po::value<int>(&historyLength));
	// controls
	desc.add_options()("controls.loadConfig", po::value<char>(&control_loadConfig));

}

void Config::parseOptions(po::variables_map &vm) {

	// color
	redHSVmin = cv::Scalar(
			vm["red.hmin"].as<int>(),
			vm["red.smin"].as<int>(),
			vm["red.vmin"].as<int>());

	blueHSVmin = cv::Scalar(
			vm["blue.hmin"].as<int>(),
			vm["blue.smin"].as<int>(),
			vm["blue.vmin"].as<int>());
	greenHSVmin = cv::Scalar(
			vm["green.hmin"].as<int>(),
			vm["green.smin"].as<int>(),
			vm["green.vmin"].as<int>());
	redHSVmax = cv::Scalar(
			vm["red.hmax"].as<int>(),
			vm["red.smax"].as<int>(),
			vm["red.vmax"].as<int>());
	blueHSVmax = cv::Scalar(
			vm["blue.hmax"].as<int>(),
			vm["blue.smax"].as<int>(),
			vm["blue.vmax"].as<int>());
	greenHSVmax = cv::Scalar(
			vm["green.hmax"].as<int>(),
			vm["green.smax"].as<int>(),
			vm["green.vmax"].as<int>());

}

bool Config::loadConfig(std::string filename) {
	po::variables_map varMap;
	po::options_description desc("Options");



	std::ifstream file(filename.c_str());

	if(file.is_open()) {
		addOptions(desc);
		po::store(po::parse_config_file(file,desc), varMap);
		po::notify(varMap);
		parseOptions(varMap);
		return true;
	} else {
		std::cout << "Could not open configuration file \"" << filename << "\"" << std::endl;
		return false;
	}

}


} /* namespace util */
