# xtext

Overlay text on your X display, that's it.

## Usage

Pipe text into xtext and give it the coordinates of where to display.

```bash
input | xtext x y
```

Standard xtext structure.

```bash
echo "Hello World!" | xtext 0 0
```

xtext is able to handle streams of text, allowing you to update what it displays.

```bash
while true; do echo $(date +%M:%S); sleep 1; done | xtext 0 0
```

## Formatting

xtext also takes advantage of Pango, a library for rendering and organizing text. Pango comes with its own markup language allowing us to add attributes to the displayed text.

```bash
echo "<span color='red'>Hello World!</span>" | xtext 0 0
```

The Gnome Developer [docs](https://developer.gnome.org/pango/stable/PangoMarkupFormat.html) have a page dedicated to all the different Pango markup attributes. Changing the attributes becomes very powerful when paired with an updating stream of text. You can make some really interesting UI elements by changing how the text is displayed on the fly. Take a look at the 'examples/' if want to find a place to start.

## Limitations

+ Only aligned left
+ Always uses a transparent background (Requires compositer)
+ No argument switches
+ No configuration file

## Resources

### xlib/x11

### Cairo

### Pango