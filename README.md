# RayTracing-One-Week

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

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

```

### Running

```bash
# Generate a `output.tga` image file
./RayTracing
```

## Features (To Be) Implemented

Following the book's chapters:

- [x] **Chapter 1-2**: Basic PPM output and background gradient
- [x] **Chapter 3-4**: Ray casting and sphere intersection
- [x] **Chapter 5-6**: Surface normals and multiple objects
- [ ] **Chapter 7-8**: Antialiasing and diffuse materials
- [ ] **Chapter 9-10**: Metal materials and fuzzy reflection
- [ ] **Chapter 11-12**: Dielectric materials and refraction
- [x] **Chapter 13**: Positionable camera
- [x] **Chapter 14**: Depth of field

## Dependencies

This project uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) for dependency management. Dependencies are automatically downloaded during the build process.

Current dependencies:

-   None (pure C99 implementation)

## Performance Notes

-   The raytracer is CPU-intensive and currently single-threaded.
-   Rendering times depend on image resolution and sample count.
-   For faster previews, reduce `SAMPLES_PER_PIXEL` and image dimensions
-   Release builds are significantly faster than debug builds

## Contributing

1.  Fork the repository
2.  Create a feature branch (`git checkout -b feature/amazing-feature`)
3.  Commit your changes (`git commit -m 'Add amazing feature'`)
4.  Push to the branch (`git push origin feature/amazing-feature`)
5.  Open a Pull Request

## Learning Resources

-   [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) - The original book
-   [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) - Follow-up book
-   [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) - Advanced techniques

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

-   **Peter Shirley** for the excellent "Ray Tracing in One Weekend" book series
-   **The raytracing community** for continuous inspiration and knowledge sharing
-   **Contributors** who help improve this implementation

