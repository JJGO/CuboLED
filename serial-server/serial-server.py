from flask import Flask, request, render_template
import flask
import serial

app = Flask(__name__)
app.debug = True


def init_serial():
    global ser
    ser = serial.Serial('/dev/tty.usbserial-DA00E4N5',
                        baudrate=38400,
                        bytesize=serial.EIGHTBITS,
                        parity=serial.PARITY_NONE,
                        stopbits=serial.STOPBITS_ONE,
                        timeout=1.0)


def send_command(cmd):
    try:
        ser.write(('\r'+cmd+'\r').encode())
    except:
        print('Error writing on port')


@app.route('/')
def cubo():
    cmd = request.args.get('cmd')

    sent = None
    if cmd is not None:
        msg = request.args.get('msg')
        vel = request.args.get('vel')
        if cmd.startswith('F'):
            cmd += ' ' + msg

        print(repr(cmd))
        send_command(cmd)
        sent = 'Enviado'

        if vel is not None and not cmd.startswith('E0'):
            if int(vel) > 0:
                P = 1200 - 100 * (int(vel))
                print(P)
                send_command('P{0:d}'.format(P))

    return render_template('index.html', sent=sent)

if __name__ == '__main__':
    init_serial()
    try:
        app.run(port=8080)
    except:
        ser.close()