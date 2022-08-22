# GUI Design for Cockpit using ArcGIS

## System Setup

* Install [Qt Framework _**5.15.2**_](https://developers.arcgis.com/qt/install-and-set-up/#download-the-qt-framework-for-linux-and-qt-creator) and        [ArcGIS Runtime SDK __*v100.14.1*__ for Qt API](https://developers.arcgis.com/qt/install-and-set-up/#install-arcgis-runtime-api-for-qt) on Linux.

* Open a new terminal and paste the following command:

  `mkdir -p ~/dev/cpp/arcgis && cd ~/dev/cpp/arcgis`

  Then clone the repositories:

  `git clone https://git.unibw.de/angewandte-ki-f-r-dynamische-systeme/cockpit-arcgis.git && git clone https://git.unibw.de/angewandte-ki-f-r-dynamische-systeme/cockpit-helper.git`

* run this command to install GNU Compiler Collection, ZeroMQ3 and OpenGL libraries:

`sudo apt-get install build-essential && sudo apt-get install libzmq3-dev && sudo apt install libgl1-mesa-dev`

* Open the Qt Creator and then click on __*Open Project*__. Go to your cloned folder cockpit-arcgis and select the .pro file. 

* Create your own developer account on this website: 
  https://developers.arcgis.com/

* Go to your Dashboard and copy your __*API Key*__, then replace it with the value of __*data_url*__ key in param/gui_param_file.yaml file.

* Go to Layers section and click on __*Import Data*__, select the zip file from the repo. Then copy the Url of a layer and  replace it with the value of __*data_url*__ key in param/gui_param_file.yaml file.

* Meanings of the layer data can be found here (from page 8):
https://tlbg.thueringen.de/fileadmin/TLBG/Vertrieb/testdaten/dlm/4-4_03_basis-dlm-aaa.pdf

* Install anaconda, create a new conda environment and activate it:

`conda create -n simauto`
`conda activate simauto`

Afterwards, go to your other cloned folder cockpit-helper and run the following command:

`conda env update -n simauto --file environment.yaml && sudo apt-get install wmctrl scrot gawk && pip install guibot pyautogui opencv-python && chmod +x dumpxml.sh getwinidbyname.sh`

* Install Air Manager:
https://siminnovations.com/wiki/index.php?title=Air_Manager_4.x_Installation
 
* Install XPlaneConnect.zip file:
https://github.com/nasa/XPlaneConnect/releases/tag/v1.3-rc5

Unzip the .zip archive and copy the folder to the plugin directory (`[X-Plane Directory]/Resources/plugins/`)

* Install Flight Simulator Plugins: 
https://siminnovations.com/wiki/index.php?title=Flight_Simulator_Plugins_Installation 

* To run the whole project:
  - Run XPlane11 simulator
  - execute dumpxml.sh in cockpit-helper folder from a terminal:

    `./dumpxml.sh`

  or to test it quickly: `python3 read_gps.py`

  - Open Qt Creator and run the project

## Known Problems
Air Manager CPU usage is changing when 75" display is connected to the computer. It overloads the CPU, and this situation affects communication pipeline and GUI performance.   

## Contributions

* Before a contribution or adding a new feature, a corresponding issue should be created first. This issue should clarify what the contribution will be about.

* camelCase case type is being used for class, variable and function names.

* All commits should follow this specific pattern: __*Issue\-\d+*__
  Example: created layout issue-2

* Merge requests must be approved by at least one developer. 

## ArcGIS Runtime API for Qt

* Main Page:
https://developers.arcgis.com/qt/

* C++ API Reference:
https://developers.arcgis.com/qt/cpp/api-reference/

## ArcGIS Developer Glossary

* https://developers.arcgis.com/documentation/glossary/

## Tutorials

* Tutorials for ArcGIS Runtime API for Qt:
https://developers.arcgis.com/qt/tutorials/

* Sample codes for ArcGIS Runtime API for Qt in C++
https://developers.arcgis.com/qt/cpp/sample-code/

* Tutorial for the interaction with ArcGIS online GUI:
https://learn.arcgis.com/de/projects/get-started-with-arcgis-online/

## Best Practices

* ArcGIS Best Practices:
https://developers.arcgis.com/qt/programming-patterns/best-practices/

* Qt Creator Best Practices:
https://web.stanford.edu/dept/cs_edu/resources/qt/recommended-settings


