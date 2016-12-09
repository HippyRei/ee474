package com.example.group10.tankdrive;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.res.Configuration;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.UUID;

import static java.lang.Math.floor;
import static java.lang.Math.round;
import static java.sql.DriverManager.println;

public class MainActivity extends AppCompatActivity {
    protected int requestCode = 0;
    protected BluetoothAdapter mBluetoothAdapter;
    public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    protected ConnectThread ct = null;
    protected SeekBar speed = null;

    protected View.OnTouchListener uClick = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                forward(v);
                v.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                stop(v);
                v.setPressed(false);
            }
            return true;
        }
    };

    protected View.OnTouchListener dClick = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                back(v);
                v.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                stop(v);
                v.setPressed(false);
            }
            return true;
        }
    };

    protected View.OnTouchListener lClick = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                left(v);
                v.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                stop(v);
                v.setPressed(false);
            }
            return true;
        }
    };

    protected View.OnTouchListener rClick = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                right(v);
                v.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                stop(v);
                v.setPressed(false);
            }
            return true;
        }
    };

    protected View.OnTouchListener pClick = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if (v.isPressed()) {
                    power_off(v);
                } else {
                    ct.run();
                }
                v.setPressed(!v.isPressed());
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                stop(v);
            }
            return true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d("CREATE", "Creating");

        ImageButton up = (ImageButton) findViewById(R.id.up);
        ImageButton down = (ImageButton) findViewById(R.id.down);
        ImageButton left = (ImageButton) findViewById(R.id.left);
        ImageButton right = (ImageButton) findViewById(R.id.right);
        ImageButton power = (ImageButton) findViewById(R.id.power);

        up.setOnTouchListener(uClick);
        down.setOnTouchListener(dClick);
        left.setOnTouchListener(lClick);
        right.setOnTouchListener(rClick);
        power.setOnTouchListener(pClick);

        speed = (SeekBar) findViewById(R.id.speed);

        BluetoothDevice tank = null;

        if (ct == null) {
            mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            if (mBluetoothAdapter == null) {
                return;
            }

            if (!mBluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, requestCode);
            }

            Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
            // If there are paired devices
            if (pairedDevices.size() > 0) {
                // Loop through paired devices
                for (BluetoothDevice device : pairedDevices) {
                    tank = device;
                }
            }

            if (tank != null) {
                ct = ConnectThread.create(tank);
            }
        }
    }

    /*
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        setContentView(R.layout.activity_main);
        ImageButton up = (ImageButton) findViewById(R.id.up);

        up.setOnTouchListener(uClick);
    }
    */

    public void forward(View view) {
        Log.d("DRIVE", "Forward");
        byte duty = (byte) floor(((float) speed.getProgress() / speed.getMax()) * 126.0);
        byte[] bytes = new byte[4];
        bytes[0] = (byte) (0x80 | duty);
        bytes[1] = (byte) (0x80 | duty);
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    public void left(View view) {
        Log.d("DRIVE", "Left");
        byte duty = (byte) floor(((float) speed.getProgress() / speed.getMax()) * 126.0);
        byte[] bytes = new byte[4];
        bytes[0] = duty;
        bytes[1] = (byte) (0x80 | duty);
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    public void right(View view) {
        Log.d("DRIVE", "Right");
        byte duty = (byte) floor(((float) speed.getProgress() / speed.getMax()) * 126.0);
        byte[] bytes = new byte[4];
        bytes[0] = (byte) (0x80 | duty);
        bytes[1] = duty;
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    public void back(View view) {
        Log.d("DRIVE", "Backward");
        byte duty = (byte) floor(((float) speed.getProgress() / speed.getMax()) * 126.0);
        byte[] bytes = new byte[4];
        bytes[0] = duty;
        bytes[1] = duty;
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    public void stop(View view) {
        Log.d("CREATE", "Stop");
        byte[] bytes = new byte[4];
        bytes[0] = (byte) 0x00;
        bytes[1] = (byte) 0x00;
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    public void power_off(View view) {
        byte[] bytes = new byte[4];
        bytes[0] = (byte) 0xFF;
        bytes[1] = (byte) 0x00;
        bytes[2] = (byte) 0x0D;
        bytes[3] = (byte) 0x0A;

        ct.write(bytes);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        this.requestCode = requestCode;
    }

}