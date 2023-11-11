# VespertineWebHub

Welcome to VespertineWebHub, a project that seamlessly integrates the Vue.js app "Gaby.tech" with the ESP-IDF framework, providing a sophisticated web server for handling logic and serving the Vue.js application.

## Overview

VespertineWebHub combines the elegance of Vue.js with the robust capabilities of ESP-IDF to create a dynamic web server capable of serving and managing the Gaby.tech app. The project's name reflects a blend of twilight sophistication ("Vespertine"), the ESP-IDF framework ("esp"), and the central role of handling web logic ("WebHub").

## Features

## DNS Integration: VortexDNS

VespertineWebHub incorporates the VortexDNS component to seamlessly manage Domain Name System (DNS) resolution. This component ensures a smooth interaction between the ESP-IDF framework and Vue.js, serving as a pivotal element in the overall architecture.

### Features

- **Dynamic Resolution:** Orchestrates DNS resolution dynamically, enhancing the web server's capabilities.
  
- **Configurable Settings:** Enables easy configuration of DNS settings, allowing users to tailor the resolution process to their specific needs.

### Usage

To leverage VortexDNS in VespertineWebHub, follow these steps:

1. Ensure that the VortexDNS component is included in your project's `CMakeLists.txt` file.
   
   ```cmake
   idf_component_register(SRCS "vortex_dns.c"
                          INCLUDE_DIRS "dns")

## Getting Started

To get started with VespertineWebHub, follow these steps:

1. Clone the repository: `git clone https://github.com/veselinovStf/VespertineWebHub.git`
2. Navigate to the project directory: `cd VespertineWebHub`
3. [Additional Setup Steps if any]

## Usage

[Include instructions on how to use or deploy the VespertineWebHub. Provide any configuration steps, if necessary.]

```bash
# Example command or code snippet