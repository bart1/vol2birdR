#include <Rcpp.h>
#include <memory>
#include <string.h>

extern "C" {
#include "libvol2bird/libvol2bird.h"
#include "libvol2bird/constants.h"
}

using namespace Rcpp;
#include "vol2birdR_types.h"

//' @export PolarVolume
class PolarVolume {
private:
  PolarVolume_t *_polarvolume;
public:
  PolarVolume() {
    _polarvolume = (PolarVolume_t*)RAVE_OBJECT_NEW(&PolarVolume_TYPE);
    if (_polarvolume == NULL) {
      throw Rcpp::exception(std::string("Could not create internal polar volume instance").c_str());
    }
  }
  PolarVolume(PolarVolume_t *polarvolume) {
    _polarvolume = (PolarVolume_t*) RAVE_OBJECT_COPY(polarvolume);
  }
  PolarVolume(const PolarVolume &c) {
    _polarvolume = (PolarVolume_t*) RAVE_OBJECT_COPY(c._polarvolume);
  }
  virtual ~PolarVolume() {
    RAVE_OBJECT_RELEASE(_polarvolume);
  }
  PolarVolume_t* get() {
    return _polarvolume;
  }
  int getNumberOfScans() {
    return PolarVolume_getNumberOfScans(_polarvolume);
  }
};
//RCPP_EXPOSED_CLASS(PolarVolume)

//' @export RaveIO
class RaveIO {
private:
  RaveIO_t *_raveio;
  PolarVolume *_polarvolume;
public:
  RaveIO() {
    _raveio = (RaveIO_t*)RAVE_OBJECT_NEW(&RaveIO_TYPE);
    _polarvolume = NULL;
  }
  RaveIO(std::string filename, bool lazyLoading = FALSE) {
    _polarvolume = NULL;
    _raveio = RaveIO_open(filename.c_str(), lazyLoading ? 1 : 0, NULL);
    if (_raveio == NULL) {
      throw Rcpp::exception(std::string("Could not open file").c_str());
    }
  }
  virtual ~RaveIO() {
    RAVE_OBJECT_RELEASE(_raveio);
  }

  PolarVolume object() {
    if (_polarvolume == NULL) {
      RaveCoreObject *object = (RaveCoreObject*) RaveIO_getObject(_raveio);
      if (RAVE_OBJECT_CHECK_TYPE(object, &PolarVolume_TYPE)) {
        _polarvolume = new PolarVolume((PolarVolume_t*) object);
      } else {
        throw Rcpp::exception(std::string("Could not return object").c_str());
      }
    }
    return *_polarvolume;
  }

  CharacterVector filename() {
    return CharacterVector::create(Rcpp::String(RaveIO_getFilename(_raveio)));
  }
};

//' @export Vol2BirdConfig
class Vol2BirdConfig {
private:
  vol2bird_t _alldata;
  void initialize_config(vol2bird_t *alldata) {
    strcpy(alldata->misc.filename_pvol, "");
    alldata->options.elevMin = 0.0;
    alldata->options.elevMax = 45.0;
    strcpy(alldata->options.dbzType, "DBZH");
    alldata->options.azimMax = 360.0;
    alldata->options.azimMin = 0.0;
    alldata->options.layerThickness = 200.0;
    alldata->options.nLayers = 25;
    alldata->options.rangeMax = 35000.0;
    alldata->options.rangeMin = 5000.0;
    alldata->options.radarWavelength = 5.3;
    alldata->options.useClutterMap = FALSE;
    alldata->options.clutterValueMin = 0.1;
    strcpy(alldata->options.clutterMap, "");
    alldata->options.printDbz = FALSE;
    alldata->options.printDealias = FALSE;
    alldata->options.printVrad = FALSE;
    alldata->options.printRhohv = FALSE;
    alldata->options.printTex = FALSE;
    alldata->options.printCell = FALSE;
    alldata->options.printCellProp = FALSE;
    alldata->options.printClut = FALSE;
    alldata->options.printOptions = FALSE;
    alldata->options.printProfileVar = FALSE;
    alldata->options.printPointsArray = FALSE;
    alldata->options.fitVrad = TRUE;
    alldata->options.exportBirdProfileAsJSONVar = FALSE;
    alldata->options.minNyquist = 5.0;
    alldata->options.maxNyquistDealias = 25.0;
    alldata->options.birdRadarCrossSection = 11.0;
    alldata->options.cellStdDevMax = 0.0; // cfg_getfloat(*cfg,"STDEV_CELL");
    alldata->options.stdDevMinBird = 2.0;
    alldata->options.etaMax = 36000.0;
    alldata->options.cellEtaMin = 11500.0;
    alldata->options.requireVrad = FALSE;
    alldata->options.dealiasVrad = FALSE;
    alldata->options.dealiasRecycle = FALSE;
    alldata->options.dualPol = TRUE;
    alldata->options.singlePol = TRUE;
    alldata->options.dbzThresMin = 0.0;
    alldata->options.rhohvThresMin = 0.95;
    alldata->options.resample = FALSE;
    alldata->options.resampleRscale = 500.0;
    alldata->options.resampleNbins = 100;
    alldata->options.resampleNrays = 360;
    alldata->options.mistNetNElevs = 5;
    alldata->options.mistNetElevs[0] = 0.5;
    alldata->options.mistNetElevs[1] = 1.5;
    alldata->options.mistNetElevs[2] = 2.5;
    alldata->options.mistNetElevs[3] = 3.5;
    alldata->options.mistNetElevs[4] = 4.5;
    alldata->options.mistNetElevsOnly = TRUE;
    alldata->options.useMistNet = TRUE;
    strcpy(alldata->options.mistNetPath, "/opt/vol2bird/etc/mistnet_nexrad.pt");

    // ------------------------------------------------------------- //
    //              vol2bird options from constants.h                //
    // ------------------------------------------------------------- //
    alldata->constants.areaCellMin = AREACELL;
    alldata->constants.cellClutterFractionMax = CLUTPERCCELL;
    alldata->constants.chisqMin = CHISQMIN;
    alldata->constants.fringeDist = FRINGEDIST;
    alldata->constants.nBinsGap = NBINSGAP;
    alldata->constants.nPointsIncludedMin = NDBZMIN;
    alldata->constants.nNeighborsMin = NEIGHBORS;
    alldata->constants.nObsGapMin = NOBSGAPMIN;
    alldata->constants.nAzimNeighborhood = NTEXBINAZIM;
    alldata->constants.nRangNeighborhood = NTEXBINRANG;
    alldata->constants.nCountMin = NTEXMIN;
    alldata->constants.refracIndex = REFRACTIVE_INDEX_OF_WATER;
    alldata->constants.absVDifMax = VDIFMAX;
    alldata->constants.vradMin = VRADMIN;

    // ------------------------------------------------------------- //
    //       some other variables, derived from user options         //
    // ------------------------------------------------------------- //
    alldata->misc.rCellMax = alldata->options.rangeMax + RCELLMAX_OFFSET;
    alldata->misc.nDims = 2;
    alldata->misc.nParsFitted = 3;

    // the following settings depend on wavelength, will be set in Vol2birdSetup
    alldata->misc.dbzFactor = NAN;
    alldata->misc.dbzMax = NAN;
    alldata->misc.cellDbzMin = NAN;

    alldata->misc.loadConfigSuccessful = FALSE;
  }

public:
  Vol2BirdConfig() {
    initialize_config(&_alldata);
  }

  vol2bird_t* alldata() {
    return (&_alldata);
  }

  double get_elevMin() {
    return _alldata.options.elevMin;
  }
  void set_elevMin(double e) {
    _alldata.options.elevMin = e;
  }

  double get_elevMax() {
    return _alldata.options.elevMax;
  }
  void set_elevMax(double e) {
    _alldata.options.elevMax = e;
  }

  std::string get_dbzType() {
    return std::string(_alldata.options.dbzType);
  }
  void set_dbzType(std::string v) {
    strcpy(_alldata.options.dbzType, v.c_str());
  }

  double get_azimMax() {
    return _alldata.options.azimMax;
  }
  void set_azimMax(double v) {
    _alldata.options.azimMax = v;
  }

  double get_azimMin() {
    return _alldata.options.azimMin;
  }
  void set_azimMin(double v) {
    _alldata.options.azimMin = v;
  }

  double get_layerThickness() {
    return _alldata.options.layerThickness;
  }
  void set_layerThickness(double v) {
    _alldata.options.layerThickness = v;
  }

  int get_nLayers() {
    return _alldata.options.nLayers;
  }
  void set_nLayers(int v) {
    _alldata.options.nLayers = v;
  }

  double get_rangeMax() {
    return _alldata.options.rangeMax;
  }
  void set_rangeMax(double v) {
    _alldata.options.rangeMax = v;
    _alldata.misc.rCellMax = v + RCELLMAX_OFFSET;
  }

  double get_rangeMin() {
    return _alldata.options.rangeMin;
  }
  void set_rangeMin(double v) {
    _alldata.options.rangeMin = v;
  }

  double get_radarWavelength() {
    return _alldata.options.radarWavelength;
  }
  void set_radarWavelength(double v) {
    _alldata.options.radarWavelength = v;
  }

  bool get_useClutterMap() {
    return _alldata.options.useClutterMap == TRUE ? true : false;
  }
  void set_useClutterMap(bool v) {
    _alldata.options.useClutterMap = v == true ? TRUE : FALSE;
  }

  double get_clutterValueMin() {
    return _alldata.options.clutterValueMin;
  }
  void set_clutterValueMin(double v) {
    _alldata.options.clutterValueMin = v;
  }

  std::string get_clutterMap() {
    return std::string(_alldata.options.clutterMap);
  }
  void set_clutterMap(std::string v) {
    strcpy(_alldata.options.clutterMap, v.c_str());
  }

  bool get_printDbz() {
    return _alldata.options.printDbz == TRUE ? true : false;
  }
  void set_printDbz(bool v) {
    _alldata.options.printDbz = v == true ? TRUE : FALSE;
  }

  bool get_printDealias() {
    return _alldata.options.printDealias == TRUE ? true : false;
  }
  void set_printDealias(bool v) {
    _alldata.options.printDealias = v == true ? TRUE : FALSE;
  }

  bool get_printVrad() {
    return _alldata.options.printVrad == TRUE ? true : false;
  }
  void set_printVrad(bool v) {
    _alldata.options.printVrad = v == true ? TRUE : FALSE;
  }

  bool get_printRhohv() {
    return _alldata.options.printRhohv == TRUE ? true : false;
  }
  void set_printRhohv(bool v) {
    _alldata.options.printRhohv = v == true ? TRUE : FALSE;
  }

  bool get_printTex() {
    return _alldata.options.printTex == TRUE ? true : false;
  }
  void set_printTex(bool v) {
    _alldata.options.printTex = v == true ? TRUE : FALSE;
  }

  bool get_printCell() {
    return _alldata.options.printCell == TRUE ? true : false;
  }
  void set_printCell(bool v) {
    _alldata.options.printCell = v == true ? TRUE : FALSE;
  }

  bool get_printCellProp() {
    return _alldata.options.printCellProp == TRUE ? true : false;
  }
  void set_printCellProp(bool v) {
    _alldata.options.printCellProp = v == true ? TRUE : FALSE;
  }

  bool get_printClut() {
    return _alldata.options.printClut == TRUE ? true : false;
  }
  void set_printClut(bool v) {
    _alldata.options.printClut = v == true ? TRUE : FALSE;
  }

  bool get_printOptions() {
    return _alldata.options.printOptions == TRUE ? true : false;
  }
  void set_printOptions(bool v) {
    _alldata.options.printOptions = v == true ? TRUE : FALSE;
  }

  bool get_printProfileVar() {
    return _alldata.options.printProfileVar == TRUE ? true : false;
  }
  void set_printProfileVar(bool v) {
    _alldata.options.printProfileVar = v == true ? TRUE : FALSE;
  }

  bool get_printPointsArray() {
    return _alldata.options.printPointsArray == TRUE ? true : false;
  }
  void set_printPointsArray(bool v) {
    _alldata.options.printPointsArray = v == true ? TRUE : FALSE;
  }

  bool get_fitVrad() {
    return _alldata.options.fitVrad == TRUE ? true : false;
  }
  void set_fitVrad(bool v) {
    _alldata.options.fitVrad = v == true ? TRUE : FALSE;
  }

  bool get_exportBirdProfileAsJSONVar() {
    return _alldata.options.exportBirdProfileAsJSONVar == TRUE ? true : false;
  }
  void set_exportBirdProfileAsJSONVar(bool v) {
    _alldata.options.exportBirdProfileAsJSONVar = v == true ? TRUE : FALSE;
  }

  double get_minNyquist() {
    return _alldata.options.minNyquist;
  }
  void set_minNyquist(double v) {
    _alldata.options.minNyquist = v;
  }

  double get_maxNyquistDealias() {
    return _alldata.options.maxNyquistDealias;
  }
  void set_maxNyquistDealias(double v) {
    _alldata.options.maxNyquistDealias = v;
  }

  double get_birdRadarCrossSection() {
    return _alldata.options.birdRadarCrossSection;
  }
  void set_birdRadarCrossSection(double v) {
    _alldata.options.birdRadarCrossSection = v;
  }

  double get_cellStdDevMax() {
    return _alldata.options.cellStdDevMax;
  }
  void set_cellStdDevMax(double v) {
    _alldata.options.cellStdDevMax = v;
  }

  double get_stdDevMinBird() {
    return _alldata.options.stdDevMinBird;
  }
  void set_stdDevMinBird(double v) {
    _alldata.options.stdDevMinBird = v;
  }

  double get_etaMax() {
    return _alldata.options.etaMax;
  }
  void set_etaMax(double v) {
    _alldata.options.etaMax = v;
  }

  double get_cellEtaMin() {
    return _alldata.options.cellEtaMin;
  }
  void set_cellEtaMin(double v) {
    _alldata.options.cellEtaMin = v;
  }

  bool get_requireVrad() {
    return _alldata.options.requireVrad == TRUE ? true : false;
  }
  void set_requireVrad(bool v) {
    _alldata.options.requireVrad = v == true ? TRUE : FALSE;
  }

  bool get_dealiasVrad() {
    return _alldata.options.dealiasVrad == TRUE ? true : false;
  }
  void set_dealiasVrad(bool v) {
    _alldata.options.dealiasVrad = v == true ? TRUE : FALSE;
  }

  bool get_dealiasRecycle() {
    return _alldata.options.dealiasRecycle == TRUE ? true : false;
  }
  void set_dealiasRecycle(bool v) {
    _alldata.options.dealiasRecycle = v == true ? TRUE : FALSE;
  }

  bool get_dualPol() {
    return _alldata.options.dualPol == TRUE ? true : false;
  }
  void set_dualPol(bool v) {
    _alldata.options.dualPol = v == true ? TRUE : FALSE;
  }

  bool get_singlePol() {
    return _alldata.options.singlePol == TRUE ? true : false;
  }
  void set_singlePol(bool v) {
    _alldata.options.singlePol = v == true ? TRUE : FALSE;
  }

  double get_dbzThresMin() {
    return _alldata.options.dbzThresMin;
  }
  void set_dbzThresMin(double v) {
    _alldata.options.dbzThresMin = v;
  }

  double get_rhohvThresMin() {
    return _alldata.options.rhohvThresMin;
  }
  void set_rhohvThresMin(double v) {
    _alldata.options.rhohvThresMin = v;
  }

  bool get_resample() {
    return _alldata.options.resample == TRUE ? true : false;
  }
  void set_resample(bool v) {
    _alldata.options.resample = v == true ? TRUE : FALSE;
  }

  bool get_resampleRscale() {
    return _alldata.options.resampleRscale == TRUE ? true : false;
  }
  void set_resampleRscale(bool v) {
    _alldata.options.resampleRscale = v == true ? TRUE : FALSE;
  }

  bool get_resampleNbins() {
    return _alldata.options.resampleNbins == TRUE ? true : false;
  }
  void set_resampleNbins(bool v) {
    _alldata.options.resampleNbins = v == true ? TRUE : FALSE;
  }

  bool get_resampleNrays() {
    return _alldata.options.resampleNrays == TRUE ? true : false;
  }
  void set_resampleNrays(bool v) {
    _alldata.options.resampleNrays = v == true ? TRUE : FALSE;
  }

  int get_mistNetNElevs() {
    return _alldata.options.mistNetNElevs;
  }
  void set_mistNetNElevs(int v) {
    _alldata.options.mistNetNElevs = v;
  }

  NumericVector get_mistNetElevs() {
    NumericVector v(_alldata.options.mistNetNElevs);
    for (int i = 0; i < _alldata.options.mistNetNElevs; i++) {
      v[i] = _alldata.options.mistNetElevs[i];
    }
    return v;
  }

  void set_mistNetElevs(NumericVector v) {
    int nlen = v.size();
    for (int i = 0; i < nlen; i++) {
      _alldata.options.mistNetElevs[i] = v[i];
    }
    _alldata.options.mistNetNElevs = nlen;
  }

  void set_mistNetElevsOnly(bool v) {
    _alldata.options.mistNetElevsOnly = v == true ? TRUE : FALSE;
  }
  bool get_mistNetElevsOnly() {
    return _alldata.options.mistNetElevsOnly == TRUE ? true : false;
  }

  void set_useMistNet(bool v) {
    _alldata.options.useMistNet = v == true ? TRUE : FALSE;
  }
  bool get_useMistNet() {
    return _alldata.options.useMistNet == TRUE ? true : false;
  }

  std::string get_mistNetPath() {
    return std::string(_alldata.options.mistNetPath);
  }
  void set_mistNetPath(std::string v) {
    strcpy(_alldata.options.mistNetPath, v.c_str());
  }

  double get_constant_areaCellMin() {
    return _alldata.constants.areaCellMin;
  }
  void set_constant_areaCellMin(double v) {
    _alldata.constants.areaCellMin = v;
  }

  double get_constant_cellClutterFractionMax() {
    return _alldata.constants.cellClutterFractionMax;
  }
  void set_constant_cellClutterFractionMax(double v) {
    _alldata.constants.cellClutterFractionMax = v;
  }

  double get_constant_chisqMin() {
    return _alldata.constants.chisqMin;
  }
  void set_constant_chisqMin(double v) {
    _alldata.constants.chisqMin = v;
  }

  double get_constant_fringeDist() {
    return _alldata.constants.fringeDist;
  }
  void set_constant_fringeDist(double v) {
    _alldata.constants.fringeDist = v;
  }

  int get_constant_nBinsGap() {
    return _alldata.constants.nBinsGap;
  }
  void set_constant_nBinsGap(int v) {
    _alldata.constants.nBinsGap = v;
  }

  int get_constant_nPointsIncludedMin() {
    return _alldata.constants.nPointsIncludedMin;
  }
  void set_constant_nPointsIncludedMin(int v) {
    _alldata.constants.nPointsIncludedMin = v;
  }

  int get_constant_nNeighborsMin() {
    return _alldata.constants.nNeighborsMin;
  }
  void set_constant_nNeighborsMin(int v) {
    _alldata.constants.nNeighborsMin = v;
  }

  int get_constant_nObsGapMin() {
    return _alldata.constants.nObsGapMin;
  }
  void set_constant_nObsGapMin(int v) {
    _alldata.constants.nObsGapMin = v;
  }

  int get_constant_nAzimNeighborhood() {
    return _alldata.constants.nAzimNeighborhood;
  }
  void set_constant_nAzimNeighborhood(int v) {
    _alldata.constants.nAzimNeighborhood = v;
  }

  int get_constant_nRangNeighborhood() {
    return _alldata.constants.nRangNeighborhood;
  }
  void set_constant_nRangNeighborhood(int v) {
    _alldata.constants.nRangNeighborhood = v;
  }

  int get_constant_nCountMin() {
    return _alldata.constants.nCountMin;
  }
  void set_constant_nCountMin(int v) {
    _alldata.constants.nCountMin = v;
  }

  double get_constant_refracIndex() {
    return _alldata.constants.refracIndex;
  }
  void set_constant_refracIndex(double v) {
    _alldata.constants.refracIndex = v;
  }

  double get_constant_absVDifMax() {
    return _alldata.constants.absVDifMax;
  }
  void set_constant_absVDifMax(double v) {
    _alldata.constants.absVDifMax = v;
  }

  double get_constant_vradMin() {
    return _alldata.constants.vradMin;
  }
  void set_constant_vradMin(double v) {
    _alldata.constants.vradMin = v;
  }

  double get_misc_dbzFactor() {
    return _alldata.misc.dbzFactor;
  }
  void set_misc_dbzFactor(double v) {
    _alldata.misc.dbzFactor = v;
  }

  double get_misc_dbzMax() {
    return _alldata.misc.dbzMax;
  }
  void set_misc_dbzMax(double v) {
    _alldata.misc.dbzMax = v;
  }

  double get_misc_cellDbzMin() {
    return _alldata.misc.cellDbzMin;
  }
  void set_misc_cellDbzMin(double v) {
    _alldata.misc.cellDbzMin = v;
  }
};

//' @export Vol2Bird
class Vol2Bird {
private:
public:
  Vol2Bird() {
  }

  virtual ~Vol2Bird() {
  }

  void process(StringVector &files, Vol2BirdConfig &config, std::string &vpOutName, std::string &volOutName) {
    PolarVolume_t *volume = NULL;
    char *fileIn[INPUTFILESMAX];
    int initSuccessful = 0;

    if (files.size() == 0) {
      throw std::invalid_argument("Must specify at least one input filename");
    }
    for (int i = 0; i < files.size(); i++) {
      fileIn[i] = (char*) files(i);
    }

    volume = vol2birdGetVolume(fileIn, files.size(), 1000000, 1);
    if (volume == NULL) {
      throw std::runtime_error("Could not read file(s)");
    }

    config.alldata()->misc.loadConfigSuccessful = TRUE; // Config is already loaded when we come here.

    if (config.alldata()->options.useClutterMap) {
      int clutterSuccessful = vol2birdLoadClutterMap(volume, config.alldata()->options.clutterMap, config.alldata()->misc.rCellMax) == 0;
      if (clutterSuccessful == FALSE) {
        RAVE_OBJECT_RELEASE(volume);
        throw std::runtime_error(std::string("Failed to load static clutter map : ") + std::string(config.alldata()->options.clutterMap));
      }
    }

    if (config.alldata()->options.resample) {
      PolarVolume_t *new_volume = PolarVolume_resample(volume, config.alldata()->options.resampleRscale, config.alldata()->options.resampleNbins,
          config.alldata()->options.resampleNrays);
      RAVE_OBJECT_RELEASE(volume);
      if (new_volume == NULL) {
        RAVE_OBJECT_RELEASE(new_volume);
        throw std::runtime_error("Failed to resample volume");
      }
      volume = new_volume;
    }

    initSuccessful = vol2birdSetUp(volume, config.alldata()) == 0;

    if (initSuccessful == FALSE) {
      RAVE_OBJECT_RELEASE(volume);
      throw std::runtime_error("Failed to initialize for processing");
    }

    if (!volOutName.empty()) {
      saveToODIM((RaveCoreObject*) volume, volOutName.c_str());
    }

    const char *date;
    const char *time;
    const char *source;

    date = PolarVolume_getDate(volume);
    time = PolarVolume_getTime(volume);
    source = PolarVolume_getSource(volume);

    {  // getter example scope begin

      int nRowsProfile = vol2birdGetNRowsProfile(config.alldata());
      int nColsProfile = vol2birdGetNColsProfile(config.alldata());

      fprintf(stdout, "# vol2bird Vertical Profile of Birds (VPB)\n");
      fprintf(stdout, "# source: %s\n", source);
      fprintf(stdout, "# polar volume input: %s\n", fileIn[0]);
      if (config.alldata()->misc.vcp > 0)
        fprintf(stdout, "# volume coverage pattern (VCP): %i\n", config.alldata()->misc.vcp);
      printf("# date   time HGHT    u      v       w     ff    dd  sd_vvp gap dbz     eta   dens   DBZH   n   n_dbz n_all n_dbz_all\n");

      float *profileBio;
      float *profileAll;

      profileBio = vol2birdGetProfile(1, config.alldata());
      profileAll = vol2birdGetProfile(3, config.alldata());

      int iRowProfile;
      int iCopied = 0;

      for (iRowProfile = 0; iRowProfile < nRowsProfile; iRowProfile++) {
        iCopied = iRowProfile * nColsProfile;
        printf("%8s %.4s ", date, time);
        printf("%4.f %6.2f %6.2f %7.2f %5.2f %5.1f %6.2f %1c %6.2f %6.1f %6.2f %6.2f %5.f %5.f %5.f %5.f\n", profileBio[0 + iCopied],
            nanify(profileBio[2 + iCopied]), nanify(profileBio[3 + iCopied]), nanify(profileBio[4 + iCopied]), nanify(profileBio[5 + iCopied]),
            nanify(profileBio[6 + iCopied]), nanify(profileAll[7 + iCopied]), profileBio[8 + iCopied] == TRUE ? 'T' : 'F', nanify(profileBio[9 + iCopied]),
            nanify(profileBio[11 + iCopied]), nanify(profileBio[12 + iCopied]), nanify(profileAll[9 + iCopied]), nanify(profileBio[10 + iCopied]),
            nanify(profileBio[13 + iCopied]), nanify(profileAll[10 + iCopied]), nanify(profileAll[13 + iCopied]));
      }

      profileAll = NULL;
      profileBio = NULL;
      free((void*) profileAll);
      free((void*) profileBio);
    } // getter scope end

    // ------------------------------------------------------------------- //
    //                 end of the getter example section                   //
    // ------------------------------------------------------------------- //

    //map vol2bird profile data to Rave profile object
    mapDataToRave(volume, config.alldata());

    if (!vpOutName.empty()) {
      int result = saveToODIM((RaveCoreObject*) config.alldata()->vp, vpOutName.c_str());
      if (result == FALSE) {
        RAVE_OBJECT_RELEASE(volume);
        throw std::runtime_error(std::string("Can not write : ") + vpOutName);
      }
    }

    vol2birdTearDown(config.alldata());
    RAVE_OBJECT_RELEASE(volume);
  }

};

RCPP_EXPOSED_CLASS_NODECL(PolarVolume)
RCPP_MODULE(PolarVolume) {
  class_<PolarVolume>("PolarVolume").constructor().method("getNumberOfScans", &PolarVolume::getNumberOfScans);
}
//RCPP_EXPOSED_AS(PolarVolume)

RCPP_EXPOSED_CLASS_NODECL(RaveIO)
RCPP_MODULE(RaveIO) {
  class_<RaveIO>("RaveIO").constructor().constructor<std::string>().method("object", &RaveIO::object).method("filename", &RaveIO::filename);
}
//RCPP_EXPOSED_AS(RaveIO)

RCPP_EXPOSED_CLASS_NODECL(Vol2BirdConfig)
RCPP_MODULE(Vol2BirdConfig) {
  class_<Vol2BirdConfig>("Vol2BirdConfig")
      .constructor()
      .property("elevMin", &Vol2BirdConfig::get_elevMin, &Vol2BirdConfig::set_elevMin)
      .property("elevMax", &Vol2BirdConfig::get_elevMax, &Vol2BirdConfig::set_elevMax)
      .property("dbzType", &Vol2BirdConfig::get_dbzType, &Vol2BirdConfig::set_dbzType)
      .property("azimMax", &Vol2BirdConfig::get_azimMax, &Vol2BirdConfig::set_azimMax)
      .property("azimMin", &Vol2BirdConfig::get_azimMin, &Vol2BirdConfig::set_azimMin)
      .property("layerThickness", &Vol2BirdConfig::get_layerThickness, &Vol2BirdConfig::set_layerThickness)
      .property("nLayers", &Vol2BirdConfig::get_nLayers, &Vol2BirdConfig::set_nLayers)
      .property("rangeMax", &Vol2BirdConfig::get_rangeMax, &Vol2BirdConfig::set_rangeMax)
      .property("rangeMin", &Vol2BirdConfig::get_rangeMin, &Vol2BirdConfig::set_rangeMin)
      .property("radarWavelength", &Vol2BirdConfig::get_radarWavelength, &Vol2BirdConfig::set_radarWavelength)
      .property("useClutterMap", &Vol2BirdConfig::get_useClutterMap, &Vol2BirdConfig::set_useClutterMap)
      .property("clutterValueMin", &Vol2BirdConfig::get_clutterValueMin, &Vol2BirdConfig::set_clutterValueMin)
      .property("clutterMap", &Vol2BirdConfig::get_clutterMap, &Vol2BirdConfig::set_clutterMap)
      .property("printDbz", &Vol2BirdConfig::get_printDbz, &Vol2BirdConfig::set_printDbz)
      .property("printDealias", &Vol2BirdConfig::get_printDealias, &Vol2BirdConfig::set_printDealias)
      .property("printVrad", &Vol2BirdConfig::get_printVrad, &Vol2BirdConfig::set_printVrad)
      .property("printRhohv", &Vol2BirdConfig::get_printRhohv, &Vol2BirdConfig::set_printRhohv)
      .property("printTex", &Vol2BirdConfig::get_printTex, &Vol2BirdConfig::set_printTex)
      .property("printCell", &Vol2BirdConfig::get_printCell, &Vol2BirdConfig::set_printCell)
      .property("printCellProp", &Vol2BirdConfig::get_printCellProp, &Vol2BirdConfig::set_printCellProp)
      .property("printClut", &Vol2BirdConfig::get_printClut, &Vol2BirdConfig::set_printClut)
      .property("printOptions", &Vol2BirdConfig::get_printOptions, &Vol2BirdConfig::set_printOptions)
      .property("printProfileVar", &Vol2BirdConfig::get_printProfileVar, &Vol2BirdConfig::set_printProfileVar)
      .property("printPointsArray", &Vol2BirdConfig::get_printPointsArray, &Vol2BirdConfig::set_printPointsArray)
      .property("fitVrad", &Vol2BirdConfig::get_fitVrad, &Vol2BirdConfig::set_fitVrad)
      .property("exportBirdProfileAsJSONVar", &Vol2BirdConfig::get_exportBirdProfileAsJSONVar, &Vol2BirdConfig::set_exportBirdProfileAsJSONVar)
      .property("minNyquist", &Vol2BirdConfig::get_minNyquist, &Vol2BirdConfig::set_minNyquist)
      .property("maxNyquistDealias", &Vol2BirdConfig::get_maxNyquistDealias, &Vol2BirdConfig::set_maxNyquistDealias)
      .property("birdRadarCrossSection", &Vol2BirdConfig::get_birdRadarCrossSection, &Vol2BirdConfig::set_birdRadarCrossSection)
      .property("cellStdDevMax", &Vol2BirdConfig::get_cellStdDevMax, &Vol2BirdConfig::set_cellStdDevMax)
      .property("stdDevMinBird", &Vol2BirdConfig::get_stdDevMinBird, &Vol2BirdConfig::set_stdDevMinBird)
      .property("etaMax", &Vol2BirdConfig::get_etaMax, &Vol2BirdConfig::set_etaMax)
      .property("cellEtaMin", &Vol2BirdConfig::get_cellEtaMin, &Vol2BirdConfig::set_cellEtaMin)
      .property("requireVrad", &Vol2BirdConfig::get_requireVrad, &Vol2BirdConfig::set_requireVrad)
      .property("dealiasVrad", &Vol2BirdConfig::get_dealiasVrad, &Vol2BirdConfig::set_dealiasVrad)
      .property("dealiasRecycle", &Vol2BirdConfig::get_dealiasRecycle, &Vol2BirdConfig::set_dealiasRecycle)
      .property("dualPol", &Vol2BirdConfig::get_dualPol, &Vol2BirdConfig::set_dualPol)
      .property("singlePol", &Vol2BirdConfig::get_singlePol, &Vol2BirdConfig::set_singlePol)
      .property("dbzThresMin", &Vol2BirdConfig::get_dbzThresMin, &Vol2BirdConfig::set_dbzThresMin)
      .property("rhohvThresMin", &Vol2BirdConfig::get_rhohvThresMin, &Vol2BirdConfig::set_rhohvThresMin)
      .property("resample", &Vol2BirdConfig::get_resample, &Vol2BirdConfig::set_resample)
      .property("resampleRscale", &Vol2BirdConfig::get_resampleRscale, &Vol2BirdConfig::set_resampleRscale)
      .property("resampleNbins", &Vol2BirdConfig::get_resampleNbins, &Vol2BirdConfig::set_resampleNbins)
      .property("resampleNrays", &Vol2BirdConfig::get_resampleNrays, &Vol2BirdConfig::set_resampleNrays)
      .property("mistNetElevs", &Vol2BirdConfig::get_mistNetElevs, &Vol2BirdConfig::set_mistNetElevs)
      .property("mistNetElevsOnly", &Vol2BirdConfig::get_mistNetElevsOnly, &Vol2BirdConfig::set_mistNetElevsOnly)
      .property("useMistNet", &Vol2BirdConfig::get_useMistNet, &Vol2BirdConfig::set_useMistNet)
      .property("mistNetPath", &Vol2BirdConfig::get_mistNetPath, &Vol2BirdConfig::set_mistNetPath)
      .property("constant_areaCellMin", &Vol2BirdConfig::get_constant_areaCellMin, &Vol2BirdConfig::set_constant_areaCellMin)
      .property("constant_cellClutterFractionMax", &Vol2BirdConfig::get_constant_cellClutterFractionMax, &Vol2BirdConfig::set_constant_cellClutterFractionMax)
      .property("constant_chisqMin", &Vol2BirdConfig::get_constant_chisqMin, &Vol2BirdConfig::set_constant_chisqMin)
      .property("constant_fringeDist", &Vol2BirdConfig::get_constant_fringeDist, &Vol2BirdConfig::set_constant_fringeDist)
      .property("constant_nBinsGap", &Vol2BirdConfig::get_constant_nBinsGap, &Vol2BirdConfig::set_constant_nBinsGap)
      .property("constant_nPointsIncludedMin", &Vol2BirdConfig::get_constant_nPointsIncludedMin, &Vol2BirdConfig::set_constant_nPointsIncludedMin)
      .property("constant_nNeighborsMin", &Vol2BirdConfig::get_constant_nNeighborsMin, &Vol2BirdConfig::set_constant_nNeighborsMin)
      .property("constant_nObsGapMin", &Vol2BirdConfig::get_constant_nObsGapMin, &Vol2BirdConfig::set_constant_nObsGapMin)
      .property("constant_nAzimNeighborhood", &Vol2BirdConfig::get_constant_nAzimNeighborhood, &Vol2BirdConfig::set_constant_nAzimNeighborhood)
      .property("constant_nRangNeighborhood", &Vol2BirdConfig::get_constant_nRangNeighborhood, &Vol2BirdConfig::set_constant_nRangNeighborhood)
      .property("constant_nCountMin", &Vol2BirdConfig::get_constant_nCountMin, &Vol2BirdConfig::set_constant_nCountMin)
      .property("constant_refracIndex", &Vol2BirdConfig::get_constant_refracIndex, &Vol2BirdConfig::set_constant_refracIndex)
      .property("constant_absVDifMax", &Vol2BirdConfig::get_constant_absVDifMax, &Vol2BirdConfig::set_constant_absVDifMax)
      .property("constant_vradMin", &Vol2BirdConfig::get_constant_vradMin, &Vol2BirdConfig::set_constant_vradMin)
      .property("misc_dbzFactor", &Vol2BirdConfig::get_misc_dbzFactor, &Vol2BirdConfig::set_misc_dbzFactor)
      .property("misc_dbzMax", &Vol2BirdConfig::get_misc_dbzMax, &Vol2BirdConfig::set_misc_dbzMax)
      .property("misc_cellDbzMin", &Vol2BirdConfig::get_misc_cellDbzMin, &Vol2BirdConfig::set_misc_cellDbzMin);
}
//RCPP_EXPOSED_AS(Vol2BirdConfig)

RCPP_EXPOSED_CLASS_NODECL(Vol2Bird)
RCPP_MODULE(Vol2Bird) {
  class_<Vol2Bird>("Vol2Bird").constructor()
  //.method("run", &Vol2Bird::run)
  .method("process", &Vol2Bird::process);
}
//RCPP_EXPOSED_AS(Vol2Bird)

