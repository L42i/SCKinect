# Contributing to SCKinect

Thank you for your interest in contributing to SCKinect! This document provides guidelines and information for contributing to the project.

## Ways to Contribute

There are several ways you can contribute to SCKinect:

1. **Reporting bugs**: If you find a bug, please report it by opening an issue on GitHub
2. **Suggesting enhancements**: Ideas for new features or improvements are welcome
3. **Improving documentation**: Help clarify and expand the documentation
4. **Submitting code**: Fix bugs or add new features through pull requests
5. **Creating examples**: Share your creative uses of SCKinect with others

## Development Setup

To set up a development environment for SCKinect:

1. Follow the [installation guide](installation.md) to install all dependencies
2. Clone the repository with Git:
   ```bash
   git clone https://github.com/L42i/SCKinect.git
   ```
3. Create a branch for your changes:
   ```bash
   git checkout -b my-feature-branch
   ```

## Building for Development

For development, it's helpful to build with debug symbols:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSC_PATH=/path/to/supercollider/source
cmake --build .
```

## Code Style Guidelines

When contributing code, please follow these guidelines:

### C++ Code

- Follow the existing naming conventions:
  - Class names: `CamelCase`
  - Methods and functions: `camelCase`
  - Variables: `camelCase` with member variables prefixed with `m` (e.g., `mVariable`)
  - Constants and enums: `ALL_CAPS`
- Add comments for non-obvious code sections
- Keep lines to a reasonable length (ideally under 100 characters)

### SuperCollider Code
- Follow the existing naming conventions:
  - Class methods: Begin with `*` (e.g., `*methodName`)
  - Instance methods: No prefix (e.g., `methodName`)
  - Variables: `camelCase`
- Document methods with appropriate argument descriptions

## Pull Request Process

1. Make sure your code follows the style guidelines
2. Update documentation as necessary
3. Test your changes thoroughly
4. Submit a pull request describing your changes
5. Wait for feedback from maintainers

## Testing

When implementing new features or fixing bugs, please include tests:

- For C++ code, add test cases if applicable
- For SuperCollider code, include example scripts that demonstrate the functionality

## Documentation

If your changes affect the user interface or add new features, please update the corresponding documentation. This might include:

- Updating method documentation in `Kinect.schelp`
- Adding examples to demonstrate new features
- Updating this MkDocs documentation

## Community Guidelines

When participating in the SCKinect community, please:

- Be respectful and inclusive of others
- Focus on the technical merits of ideas
- Help others learn and grow

## License

By contributing to SCKinect, you agree that your contributions will be licensed under the same license as the project.

## Getting Help

If you need help with contributing:

- Open an issue on GitHub with a "question" label
- Reach out to the maintainers directly (i.e. Evan Murray <evan@auraaudio.io>)

Thank you for your contributions to SCKinect! 
