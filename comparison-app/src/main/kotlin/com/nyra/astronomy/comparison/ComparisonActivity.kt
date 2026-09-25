package com.nyra.astronomy.comparison

import android.app.Activity
import android.os.Bundle
import android.widget.TextView

/** A deliberately small host for the external comparison test app. */
class ComparisonActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(TextView(this).apply {
            text = "Astronomy Engine comparison tests\nRun connectedAndroidTest."
            textSize = 18f
            setPadding(32, 32, 32, 32)
        })
    }
}
