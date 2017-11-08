
import java.awt.event.KeyEvent;

import javafx.scene.input.KeyCode;
import jssc.*;

public class SerialComm {

	SerialPort port;

	private boolean debug;  // Indicator of "debugging mode"
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
		
	public void writeByte(byte b){
		//writes bytes to the arduino
		setDebug(false);
		if(debug){
			try{
				//prints in debug mode
				port.writeByte(b);
				System.out.println("byte:" + b);
			}
			catch(SerialPortException e){
				e.printStackTrace();
			}
		}
		else{
			try{
				port.writeByte(b);
			}
			catch(SerialPortException e)
			{
				e.printStackTrace();
			}
		}
	}
	
	public boolean available() {
		//tells the user if there are bytes to read
		try {
			if(port.getInputBufferBytesCount() > 0) {
				return true;
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
		
	public byte readByte() {
		//reads bytes from port
		byte b = 0;
		
		try {
			b = port.readBytes(1)[0];
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(debug) {
			System.out.println(String.format("0x%02X", b));
			return b;
		}else {
			return b;
		}
	}
	
	public static void main(String[] args){
		SerialComm myPort;
		try {
			myPort = new SerialComm("/dev/cu.usbserial-DN02BGGE");
			while(true)
			{
				byte b = 0;
				//checks for keys pressed => use wasd
				if(StdDraw.hasNextKeyTyped()) {
					if(StdDraw.isKeyPressed(KeyEvent.VK_A)) {
						System.out.println("Left");
						b = 'l';
					}
					if(StdDraw.isKeyPressed(KeyEvent.VK_D)) {
						System.out.println("Right");
						b = 'r';
					}
					if(StdDraw.isKeyPressed(KeyEvent.VK_W)) {
						System.out.println("Up");
						b = 'u';
					}
					if(StdDraw.isKeyPressed(KeyEvent.VK_S)) {
						System.out.println("Down");
						b = 'd';
					}
				}
				
				//if detected value sends keypressed data to arduino
				if(b != 0) {
					System.out.println(b);
					myPort.writeByte(b);
					try {
						Thread.sleep(600);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				
			}
		} catch (SerialPortException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
}
