import time

anim = ['⠋', '⠇', '⠦', '⠤', '⠴', '⠸', '⠙', '⠉'] 

i = 0
while True:
    frame = anim[i % len(anim)] 
    span  = '<span font="120">{}</span>'.format(frame)
    print(span, flush=True)
    i += 1
    time.sleep(.1)
