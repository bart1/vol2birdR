
<!-- README.md is generated from README.Rmd. Please edit that file -->

# vol2birdR

**vol2birdR** is an R package for the vol2bird algorithm for calculating
vertical profiles of birds and other biological scatterers from weather
radar data.

It also provides an R interface to the MistNet convolutional neural
network for precipitation segmentation, installing PyTorch libraries and
model.

**vol2birdR** can be used as a stand-alone package, but we recommend
[bioRad](http://adokter.github.io/bioRad) as the primary user interface,
with **vol2birdR** acting as a dependency of
[bioRad](http://adokter.github.io/bioRad).

# Install

**vol2birdR** is available for all major platforms (Linux, OS X and
Windows).

For OS X and Linux the GNU Scientific Library (GSL), PROJ and HDF5
libraries need to be installed as system libraries prior to installation
of **vol2birdR**:

| System                                      | Command                                                           |
|:--------------------------------------------|:------------------------------------------------------------------|
| **OS X (using Homebrew)**                   | `brew install hdf5@1.10 proj gsl`                                 |
| **Debian-based systems (including Ubuntu)** | `sudo apt-get install libhdf5-dev libproj-dev gsl-bin libgsl-dev` |
| **Systems supporting yum and RPMs**         | `sudo yum install hdf5-devel proj-devel gsl gsl-devel`            |

Next, you can install the released version of bioRad from
[CRAN](https://CRAN.R-project.org) with:

``` r
install.packages("vol2birdR")
```

Alternatively, you can install the latest development version from
[GitHub](https://github.com/adokter/bioRad) with:

``` r
# install.packages("devtools")
devtools::install_github("adokter/vol2birdR")
```

Then load the package with:

``` r
library(vol2birdR)
```

## References:

Citation for vol2bird algorithm:

-   [**Bird migration flight altitudes studied by a network of
    operational weather
    radars**](https://doi.org/10.1098/rsif.2010.0116) Dokter AM, Liechti
    F, Stark H, Delobbe L, Tabary P, Holleman I J. R. Soc. Interface,
    **8**, 30–43, 2011, DOI
    [10.1098/rsif.2010.0116](https://doi.org/10.1098/rsif.2010.0116)

Paper describing recent algorithm extensions and the bioRad package:

-   [**bioRad: biological analysis and visualization of weather radar
    data**](https://doi.org/10.1111/ecog.04028) Dokter AM, Desmet P,
    Spaaks JH, van Hoey S, Veen L, Verlinden L, Nilsson C, Haase G,
    Leijnse H, Farnsworth A, Bouten W, Shamoun-Baranes J. Ecography,
    **42**, 852-860, 2019, DOI
    [10.1111/ecog.04028](https://doi.org/10.1111/ecog.04028)

vol2bird implements dealiasing using the torus mapping method by Haase
and Landelius:

-   [**Dealiasing of Doppler radar velocities using a torus
    mapping**](https://doi.org/10.1175/1520-0426(2004)021%3C1566:DODRVU%3E2.0.CO;2)
    Haase G, Landelius T. Journal of Atmospheric and Oceanic Technology
    **21**, 1566-1573, 2004, DOI
    [10.1175/1520-0426(2004)021\<1566:DODRVU\>2.0.CO;2](https://doi.org/10.1175/1520-0426(2004)021%3C1566:DODRVU%3E2.0.CO;2)

Use the following citation for the MistNet rain segmentation model:

-   [**MistNet: Measuring historical bird migration in the US using
    archived weather radar data and convolutional neural
    networks.**](https://doi.org/10.1111/2041-210X.13280) Lin T-Y,
    Winner K, Bernstein G, Mittal A, Dokter AM, Horton KG, Nilsson C,
    Van Doren BM, Farnsworth A, La Sorte FA, Maji S, Sheldon D. Methods
    in Ecology and Evolution, **10**, 1908-1922, 2019, DOI
    [10.1111/2041-210X.13280](https://doi.org/10.1111/2041-210X.13280)
