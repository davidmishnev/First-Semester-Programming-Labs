# PNG Image Conversion

## Task Description

Write a program that converts a **PNG image** between **palette/grayscale** and **color** formats. 

### Supported Formats

- The program must support **only 8-bit per channel images** with the following **PNG color types**:
  - **0** (Grayscale)
  - **2** (Truecolor)
  - **3** (Indexed color)

### Conversion Rules

- **Transparency (alpha channel) must be ignored** in the input image.
- The conversion must be **lossless** (i.e., the output image must be **exactly** reversible to the original).

## Implementation Requirements

- The program must accept arguments via the **command line**:

