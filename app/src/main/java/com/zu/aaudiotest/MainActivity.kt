package com.zu.aaudiotest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        btn_start.setOnClickListener {
            nStart()
        }
        btn_stop.setOnClickListener {
            nStop()
        }

        nInit()
    }

    override fun onDestroy() {
        nDestroy()
        super.onDestroy()
    }



    external fun nInit()
    external fun nStart()
    external fun nStop()
    external fun nDestroy()

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
