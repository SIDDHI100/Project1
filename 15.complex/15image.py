from PIL import Image
import matplotlib.pyplot as plt

def display_rgb_channels(input.jpeg):
    # Open the image
    img = Image.open(input.jpeg)
    
    # Split the image into Red, Green, and Blue channels
    r, g, b = img.split()
    
    # Display each channel
    plt.figure(figsize=(12, 4))
    
    # Red channel
    plt.subplot(1, 3, 1)
    plt.imshow(r, cmap="gray")
    plt.title("Red Channel")
    plt.axis("off")
    
    # Green channel
    plt.subplot(1, 3, 2)
    plt.imshow(g, cmap="gray")
    plt.title("Green Channel")
    plt.axis("off")
    
    # Blue channel
    plt.subplot(1, 3, 3)
    plt.imshow(b, cmap="gray")
    plt.title("Blue Channel")
    plt.axis("off")
    
    plt.show()

# Example usage
display_rgb_channels("input_image.jpg")
