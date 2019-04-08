# https://krazydad.com/tutorials/makecolors.php
import math, time

f = .1
i = 0

while True:

    r = round(math.sin(f * i + 0) * 127 + 128)
    g = round(math.sin(f * i + 2) * 127 + 128)
    b = round(math.sin(f * i + 4) * 127 + 128)

    col = '#%02x%02x%02x' % (r, g, b)

    print('<span color="{}"><b>bold </b><u>is</u><i> nice </i></span>'.format(col),
          flush=True)

    i += 1
    time.sleep(.1)
