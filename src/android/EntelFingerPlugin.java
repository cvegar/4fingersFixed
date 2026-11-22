package com.outsystemsenterprise.entel.PEMiEntel.cordova.plugin;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/** Native bridge for getwsq. The public response schema is unchanged: wsq, hand, img. */
public class EntelFingerPlugin extends CordovaPlugin {

    private static final String TAG = "EntelFingerPlugin-Cordova";
    private static final int BIOM_REQ_CODE = 1;
    private CallbackContext scanCallbackContext;

    @Override
    public boolean execute(String action, JSONArray args, final CallbackContext callbackContext) {
        if (!"getwsq".equals(action)) {
            callbackContext.error("\"" + action + "\" is not a recognized action.");
            return false;
        }

        // Avoid overwriting a callback while another scan is running.
        if (scanCallbackContext != null) {
            callbackContext.error("SCAN_IN_PROGRESS");
            return true;
        }

        final int leftFinger;
        final int rightFinger;
        final boolean liveness;
        try {
            JSONObject options = args.getJSONObject(0);
            leftFinger = options.getInt("leftFingerCode");
            // Preserve existing JS option spelling (righFingerCode).
            rightFinger = options.getInt("righFingerCode");
            liveness = options.getBoolean("liveness");
        } catch (JSONException | RuntimeException e) {
            Log.e(TAG, "Invalid biometric scan parameters: " + e.getClass().getSimpleName());
            callbackContext.error("INVALID_PARAMETERS");
            return true;
        }

        Log.d(TAG, "Starting 4F capture, left=" + leftFinger + ", right=" + rightFinger);
        Intent intent = new Intent(cordova.getActivity(), FourfingerActivity.class);
        intent.putExtra("LeftFinger", leftFinger);
        intent.putExtra("RightFinger", rightFinger);
        intent.putExtra("Liveness", liveness);

        scanCallbackContext = callbackContext;
        try {
            cordova.startActivityForResult(this, intent, BIOM_REQ_CODE);
        } catch (RuntimeException e) {
            Log.e(TAG, "Unable to start FourfingerActivity", e);
            sendError("START_ACTIVITY_FAILED");
        }
        return true;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode != BIOM_REQ_CODE) {
            Log.w(TAG, "Ignoring unexpected result requestCode=" + requestCode);
            return;
        }

        boolean hasExtras = data != null && data.getExtras() != null;
        Log.d(TAG, "Scan resultCode=" + resultCode + ", hasIntent=" + (data != null)
                + ", hasExtras=" + hasExtras);

        if (scanCallbackContext == null) {
            Log.w(TAG, "Scan callback already completed or unavailable");
            return;
        }

        if (resultCode != Activity.RESULT_OK) {
            String status = data == null ? null : data.getStringExtra("status");
            if (status == null || status.trim().isEmpty()) {
                sendError("CANCEL");
            } else if (status.startsWith("ERROR_") || status.equals("CAMERA_PERMISSION_DENIED")
                    || status.equals("EMPTY_BIOMETRIC_RESULTS") || status.equals("INVALID_BIOMETRIC_TEMPLATE")
                    || status.equals("FINGER_NOT_FOUND") || status.equals("ACTIVITY_RECREATED")
                    || status.equals("SDK_NOT_INITIALIZED") || status.equals("START_CAPTURE_FAILED")) {
                // Explicit technical error, useful for QA and for OutSystems error handling.
                sendError(status);
            } else {
                // Preserve the historic CANCEL code for user cancel / scan failure.
                sendError("CANCEL");
            }
            return;
        }

        if (!hasExtras) {
            sendError("EMPTY_RESULT");
            return;
        }

        try {
            final String wsq = data.getStringExtra("base64String");
            final String hand = data.getStringExtra("hand");
            final String img = data.getStringExtra("img");
            if (wsq == null || wsq.isEmpty() || hand == null || hand.isEmpty()
                    || img == null || img.isEmpty()) {
                sendError("INVALID_BIOMETRIC_RESULT");
                return;
            }

            JSONObject result = new JSONObject();
            result.put("wsq", wsq);
            result.put("hand", hand);
            result.put("img", img);

            // Do not log WSQ, image Base64 or credentials. Lengths only.
            Log.d(TAG, "Biometric response ready. hand=" + hand + ", wsqLength=" + wsq.length()
                    + ", imgLength=" + img.length());
            sendSuccess(result);
        } catch (JSONException | RuntimeException e) {
            Log.e(TAG, "Unable to prepare Cordova response", e);
            sendError("RESULT_PROCESSING_FAILED");
        }
    }

    private void sendSuccess(JSONObject result) {
        CallbackContext callback = scanCallbackContext;
        scanCallbackContext = null;
        if (callback != null) {
            callback.success(result);
        }
    }

    private void sendError(String code) {
        CallbackContext callback = scanCallbackContext;
        scanCallbackContext = null;
        Log.e(TAG, "4F returned error: " + code);
        if (callback != null) {
            callback.error(code);
        }
    }
}
