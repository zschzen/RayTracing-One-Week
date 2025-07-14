# RayTracing-One-Week

[![Build](https://github.com/zschzen/RayTracing-One-Week/actions/workflows/build.yml/badge.svg)](https://github.com/zschzen/RayTracing-One-Week/actions/workflows/build.yml)


A C99 implementation of the ray tracer from Peter Shirley's excellent book [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Features

-   **Pure C99**: Clean, portable C code following C99 standards
-   **Cross-platform**: Builds on Windows, macOS, and Linux
-   **Modern build system**: Uses CMake with CPM.cmake for dependency management
-   **Continuous Integration**: Automated testing with GitHub Actions
-   **Educational**: Follows the book's progression step by step

## Getting Started

### Prerequisites

-   C compiler supporting C99
-   CMake 3.26+

### Building

```bash
# Clone the repository
git clone https://github.com/zschzen/RayTracing-One-Week
cd RayTracing-One-Week

# Configure from the project root
cmake -S . -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DUSE_CCACHE=ON

# Build
cmake --build build
```

### Running

```bash
# Generate a `output.png` image file
./RayTracing
```

## Features (To Be) Implemented

Following the book's chapters:

* [x] **Chapter 2**: Output an Image
* [x] **Chapter 3**: The vec3 Class
* [x] **Chapter 4**: Rays, a Simple Camera, and Background
* [x] **Chapter 5**: Adding a Sphere
* [x] **Chapter 6**: Surface Normals and Multiple Objects
* [x] **Chapter 7**: Moving Camera Code Into Its Own Class
* [x] **Chapter 8**: Antialiasing
* [x] **Chapter 9**: Diffuse Materials
* [x] **Chapter 10**: Metal
* [x] **Chapter 11**: Dielectrics
* [x] **Chapter 12**: Positionable camera
* [x] **Chapter 13**: Defocus Blur

## Dependencies

This project uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) for dependency management. Dependencies are automatically downloaded during the build process.

Current dependencies:

- [stb_image.h](/external/stb_image.h), v2.30
- [stb_image_write.h](stb_image_write.h), v1.16

## Performance Notes

- The raytracer is CPU-intensive and currently single-threaded.
- Rendering times depend on image resolution and sample count.
- For faster previews, reduce `SAMPLES_PER_PIXEL` and image dimensions
- Release builds are significantly faster than debug builds

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Learning Resources

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) - The original book
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) - Follow-up book
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) - Advanced techniques

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **Peter Shirley** for the excellent "Ray Tracing in One Weekend" book series
- **The raytracing community** for continuous inspiration and knowledge sharing
- **Contributors** who help improve this implementation


