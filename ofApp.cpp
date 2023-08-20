#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	auto seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	this->face.clear();

	float threshold_1 = 0.4;
	float threshold_2 = 0.6;
	float deg_span = 0.25;
	float z_span = 1;

	float radius = 320;
	float noise_span = 0.0035;

	for (float deg = 0; deg < 360; deg += deg_span) {

		for (float z = -300; z < 4096; z += z_span) {

			auto noise_value = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, z * noise_span, ofGetFrameNum() * 0.02));
			if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

			auto noise_1 = ofNoise(glm::vec4(radius * cos((deg - deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg - deg_span) * DEG_TO_RAD) * noise_span, z * noise_span, ofGetFrameNum() * 0.02));
			auto noise_2 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z + z_span) * noise_span, ofGetFrameNum() * 0.02));
			auto noise_3 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z - z_span) * noise_span, ofGetFrameNum() * 0.02));
			auto noise_4 = ofNoise(glm::vec4(radius * cos((deg + deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg + deg_span) * DEG_TO_RAD) * noise_span, z * noise_span, ofGetFrameNum() * 0.02));

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
			vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
			vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));
			vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));

			this->face.addVertices(vertices);

			auto param = abs(0.5 - noise_value);
			for (int i = index; i < this->face.getNumVertices(); i++) {

				this->face.addColor(ofColor(255, 64, 255, ofMap(param, 0, 0.1, 255, 0)));
			}

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
			this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(180);
	
	this->face.draw();

	int start = 10;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}