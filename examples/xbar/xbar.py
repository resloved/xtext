#!/usr/bin/env python3

import sys, os, time, math
from subprocess import Popen, PIPE

class Module:

    def __init__(self, name):
        self.name = name 
        self.last = self.out()
        self.active = False
        self.alive = 0
        self.order = 0

    def out(self):
        return Popen(self.name.split(), stdout=PIPE).communicate()[0].decode('UTF-8').strip() 

folder = sys.argv[1] if len(sys.argv) > 1 else "modules/"
scripts = [ folder + file for file in os.listdir(folder) ]
modules = []

span  = "<span weight='bold'>{}</span>"
alpha = "<span alpha='{}%'>{}</span>"

for script in scripts:
    modules.append(Module(script))

uptime = 8
capped = uptime * .9
stime = time.time()
    
# Animation Curve
# http://gizma.com/easing/
def curve(now, cap, tot):

    #     _______
    #    /       \
    #   /         \
    # --           --
    # B   Plateau   A

    b = tot/2 - cap/2
    a = tot/2 + cap/2

    # Before Plateau
    if now < b:
        return round(ease_quad(now/b) * 100)
    # After Plateau (Invert)
    elif now > a: 
        return abs(100 - round(ease_quad((now-a)/b) * 100))
    # During Plateau
    return 100

# https://github.com/semitable/easing-functions/
def ease_quad(t):
    if t < 0.5:
        return 2 * t * t
    return (-2 * t * t) + (4 * t) - 1
    
order = 0

while True:

    live = []

    delta = time.time() - stime
    stime = time.time()

    for module in modules:

        # Update uptime
        module.alive += delta

        # Kill old modules
        if module.alive > uptime:
            module.active = False
            
        # Load input
        out = module.out()

        # Extend Uptime
        if module.active and module.last != out:
            if module.alive > uptime-capped:
                module.alive = uptime-capped
        # Startup
        elif module.last[0] != out[0]:
            module.active = True
            module.alive = 0
            module.order = order
            order += 1
        
        # Add live modules
        if module.active:
            module.last = out
            pct = curve(module.alive, capped, uptime) 
            pct = pct if pct > 0 else 1 # Pango error at 0
            live.append((module.order, alpha.format(pct, module.last)))

    # Order by time left (Newest furthest left)
    ordered = [ x[1] for x in sorted(live) ]
    print(span.format(' '.join(ordered)), flush=True)

    # Sleep during inactivity
    if not live:
        time.sleep(1/8)
