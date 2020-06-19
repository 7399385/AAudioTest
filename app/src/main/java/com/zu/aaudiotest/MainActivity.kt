package com.zu.aaudiotest

import android.Manifest
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.core.app.ActivityCompat
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
        checkPermission()
        nInit()


    }

    override fun onDestroy() {
        nDestroy()
        super.onDestroy()
    }

    private fun listPermissions(): ArrayList<String>
    {
        var result = ArrayList<String>()
        result.add(Manifest.permission.RECORD_AUDIO)
//        result.add(Manifest.permission.READ_EXTERNAL_STORAGE)
        result.add(Manifest.permission.WRITE_EXTERNAL_STORAGE)
        return result
    }

    private fun checkPermission()
    {
        val permissions = listPermissions()
        var allGet = true
        for(permission in permissions)
        {
            if(ActivityCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED)
            {
                Log.e(TAG, "permission $permission not granted")
                allGet = false
            }else{
                Log.d(TAG, "permission $permission granted")
            }
        }

        if(!allGet)
        {
            var permissionArray: Array<String> = Array(permissions.size){i: Int -> permissions[i] }

            ActivityCompat.requestPermissions(this, permissionArray, 33)
        }
    }



    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        when(requestCode){
            33 -> {
                for(i in grantResults.indices)
                {
                    if (grantResults[i] != PackageManager.PERMISSION_GRANTED)
                    {
                        Log.e(TAG, "permission ${permissions[i]} not granted")
                    }else{
                        Log.d(TAG, "permission ${permissions[i]} granted")
                    }
                }
            }
        }
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

        private const val TAG = "MainActivity"
    }
}
