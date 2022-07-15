# GUI Design for Cockpit using ArcGIS

## System Setup

* Install ArcGIS Runtime SDK __*v100.14.1*__ for Qt API on Linux. Follow the steps at the link:
  https://developers.arcgis.com/qt/install-and-set-up/#install-on-linux

* Install __*yaml.cpp*__ library:
  https://github.com/jbeder/yaml-cpp

* Download Qt wrapper for yaml.cpp and move it to the __*/usr/include/yaml-cpp*__ directory.
  https://gist.github.com/brcha/d392b2fe5f1e427cc8a6

* Go to your terminal and paste the following command:
  mkdir -p ~/dev/cpp/arcgis \
  && cd ~/dev/cpp/arcgis \
  && git clone https://git.unibw.de/angewandte-ki-f-r-dynamische-systeme/cockpit-arcgis.git

* Open the Qt Creator and then click on __*Open Project*__. Go to your cloned folder and select .pro file. 

* Create your own developer account on this website: 
  https://developers.arcgis.com/

* Go to your Dashboard and copy your __*API Key*__, then replace it with the value of __*data_url*__ key in param/gui_param_file.yaml file.

* Go to Layers section and click on __*Import Data*__, select the zip file from the repo. Then copy the Url of a layer and  replace it with the value of __*data_url*__ key in param/gui_param_file.yaml file.

* Meanings of the layer data can be found here:
https://tlbg.thueringen.de/fileadmin/TLBG/Vertrieb/testdaten/dlm/4-4_03_basis-dlm-aaa.pdf
(from page 8)

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


