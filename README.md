<a name="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/KaspersCult/HummingBird">
    <img src="Assets/Textures/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">HummingBird C++ Project</h3>

  <p align="center">
    An innovative toolkit for software development! 
    <br />
    <a href="https://github.com/KaspersCult/HummingBird"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <!-- <a href="https://github.com/KaspersCult/HummingBird">View Demo</a>
    · -->
    <a href="https://github.com/KaspersCult/HummingBird/issues">Report Bug</a>
    ·
    <a href="https://github.com/KaspersCult/HummingBird/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

[![PRODUCT SS][product-screenshot]](https://github.com/KaspersCult/HummingBird)

The HummingBird C++ Project is a comprehensive suite of development tools, including a core library and various applications for software development and automation tasks. It's designed to be modular and flexible, catering to a wide range of development needs.

### Built With

This project is built using a variety of technologies:

* [C++](https://www.cplusplus.com/)
* [SDL2]    og:(https://www.libsdl.org/)            (https://github.com/kasperhbo/SDL)     --> branch 2-28.x
* [Glad]    og:(https://glad.dav1d.de/)             (https://github.com/kasperhbo/Glad)    --> branch master
* [Fmt]     og:(https://fmt.dev/latest/index.html)  (https://github.com/kasperhbo/fmt)     --> branch master
* [Spdlog]  og:(https://github.com/gabime/spdlog)   (https://github.com/kasperhbo/spdlog)  --> branch v1.x
* [Imgui]   og:(https://github.com/ocornut/imgui)   (https://github.com/kasperhbo/imgui)   --> branch docking

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To get started with the HummingBird project, follow these simple steps.

### Prerequisites
At the moment only tested on an Intel Mac running macOS Montery 12.7.1, it is not tested on anything else.
Ensure you have git installed to clone the repository:
Check in your terminal with:
```sh
git --version
```

### Installation
#### Deps
1. mysql client
   1.1 Brew
     ```sh
     brew install mysql-client
     ```
   1.2 mac ports

#### Hummingbird
1. Clone the HummingBird repository:
   ```sh
   git clone https://github.com/KaspersCult/HummingBird.git —-recursive
   ```

2. go in the folder:
   ```sh 
    cd HummingBird
   ```

3. init and update the submodules:
   ```sh
    git submodule update --init --recursive
    ```
4. create a build folder and go in it:
    ```sh
    mkdir build && cd build
    ```

5. run cmake:
   ```sh
    cmake DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -S ../
    ```

6. run make with the number of cores:
   ```sh
    cmake --build . --target HummingbirdCoreEXE -- -j $(sysctl -n hw.ncpu)
    ```

7. run the executable:
   ```sh
    ./HummingbirdCoreEXE
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Use HummingBird for developing applications, creating tools, or automating tasks. For more specific examples, please refer to the [Documentation](https://github.com/KaspersCult/HummingBird).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->
## Roadmap

### Application
#### Roadmap
- [x] Core library for basic functionalities
- [x] Application for text editting -- finished
- [x] Host edittor -- in dev
- [ ] Plist launchd editor
- [ ] Integrated VNC viewer
- [ ] Terminal rewrite
- [ ] Java build tool integration
- [ ] Creating custom scripts to run your commands

#### In progress
- [x] Plist launchd editor 

### Documentation
- [ ] All documentation lol
- [ ] Proper build documentation

### Platfroms
- [x] Mac OsX Intel 12.1.7
- [ ] Mac OsX with an: M1/M2/M3
- [ ] Debian OS
- [ ] Ready for whatever Windows throws at us next – Windows 11, 12, 13, or even 68.

See the [open issues](https://github.com/KaspersCult/HummingBird/issues)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge

[license-shield]: https://img.shields.io/badge/License-GPL%20v2-blue.svg?style=for-the-badge
[license-url]: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/kasper-bruin-de-6a48ab1ba/
[product-screenshot]: images/screenshot.png
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
