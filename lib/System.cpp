/**
 ******************************************************************************* 
 * @file System.cpp
 *  @brief System class source file
 * 
 *  Top class in CALoE library. It contains several devices with their operations.
 * 
 *  Copyright (C) 2013
 *
 *  @author Miguel Jimenez Lopez <klyone@ugr.es>
 *
 *  @bug ---
 *
 *******************************************************************************
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************
 */
 
#include "System.h"

namespace caloe {

System::System() {}

System::System(const System & sys) {
	list_device = sys.list_device;
}

System System::operator=(const System & sys) {
	list_device = sys.list_device;
	
	return *this;
}

void System::addDevice(const Device & dev) {
	pair< map<string,Device>::iterator, bool > ret;
	
	// Try to insert a device into the system
	ret = list_device.insert(make_pair(dev.getName(),dev));
	
	// If the device exists already in the system, print an error and return
	if(! (ret.second)) {
		cout << "ERROR: Device "<< dev.getName() <<" already exists!"<<endl;
		cout << "IGNORING..."<<endl;
	}
}

void System::reset(string name_dev, string name_oper) {
	map<string,Device>::iterator it;
	
	// Search a device for its name
	it = list_device.find(name_dev);

	// If the device is found...
	if(it != list_device.end()) {
		//cout << "DEVICE "<<name_dev<<" found!"<<endl;
		
		// Reset the operation
		(it->second).reset(name_oper);
	}
	else { // If it is not found, print an error and return
		cout << "ERROR: Device "<<name_dev<<" not found!"<<endl;
	}
}

vector<eb_data_t> System::execute(string name_dev, string name_oper, ParamOperation & params) {
	map<string,Device>::iterator it;
	vector<eb_data_t> res;

	// Search a device for its name
	it = list_device.find(name_dev);

	// If the device is found...
	if(it != list_device.end()) {
		//cout << "DEVICE "<<name_dev<<" found!"<<endl;
		
		// Execute an operation
		res = (it->second).execute(name_oper,params);
	}
	else { // If it is not found, print an error and return
		cout << "ERROR: Device "<<name_dev<<" not found!"<<endl;
	}
	
	return res;
}

void System::loadCfgFile(string path,string name_dev) {
	Device dev;
	
	// Load a device from a configuration file
	dev.loadCfgFile(path,name_dev);
	
	// Add the new device on the system
	addDevice(dev);
}

ostream & operator<<(ostream & os, System & sys) {
	map <string,Device>::iterator it;
	
	os <<endl<<"-----------------------------------------------------------------------"<<endl;
	os <<"System "<<endl<<endl;
	
	// For each device in the system...
	for(it = sys.list_device.begin() ; it != sys.list_device.end() ; it++) {
		// Print its information
		os << it->second <<endl;
	}
	
	os <<endl<<"-----------------------------------------------------------------------"<<endl;

	return os;
}

istream & operator>>(istream & is, System & sys) {
	cout << "System: "<<endl<<endl;
	
	char cont;
	
	do {
		Device d;
		
		// Fill the new device
		is >> d;
		
		// Add the new device on the system
		sys.addDevice(d);
		
		// If you want to add other device, intro 'y' (yes) or intro 'n' (no) otherwise
		cout <<"add another device? (y/n): ";
		is >> cont;
		
	} while (cont == 'y');
	
	return is;
}

System::~System() {
}

}
