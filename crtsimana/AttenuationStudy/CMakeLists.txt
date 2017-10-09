# basic source code CMakeLists.txt

art_make(BASENAME_ONLY
	LIB_LIBRARIES 
		CRTData
		CRTProducts
		larcore_Geometry
		larcore_Geometry_Geometry_service
		lardata_Utilities
		nusimdata_SimulationBase
		${SIMULATIONBASE}
		${MF_MESSAGELOGGER}
		${MF_UTILITIES}
		${FHICLCPP}
		${CETLIB}
		${ROOT_BASIC_LIB_LIST}
		${G4_LIB_LIST}
	MODULE_LIBRARIES
		CRTData
		CRTProducts
		larcore_Geometry
		larcore_Geometry_Geometry_service
		lardata_Utilities
		nusimdata_SimulationBase
		${ART_FRAMEWORK_CORE}
		${ART_FRAMEWORK_PRINCIPAL}
		${ART_FRAMEWORK_SERVICES_REGISTRY}
		${ART_FRAMEWORK_SERVICES_BASIC}
		${ART_FRAMEWORK_SERVICES_OPTIONAL}
		${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
		${ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE}
		art_Persistency_Common
		art_Persistency_Provenance
		art_Utilities
		${MF_MESSAGELOGGER}
		${MF_UTILITIES}
		${FHICLCPP}
		${CETLIB}
		${ROOT_BASIC_LIB_LIST}
		${G4_LIB_LIST}
	)


install_headers()
install_source()
install_fhicl()
