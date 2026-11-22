package com.outsystemsenterprise.entel.PEMiEntel.cordova.plugin;

// Cordova-required packages
import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.annotation.TargetApi;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import java.nio.charset.StandardCharsets;
//import android.support.annotation.NonNull;
//import android.support.v7.app.AlertDialog;
//import android.support.v4.app.ActivityCompat;
//import android.support.v4.content.ContextCompat;

import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.veridiumid.sdk.activities.DefaultVeridiumSDKModelFactory;
import com.veridiumid.sdk.defaultdata.VeridiumSDKDataInitializer;
import com.veridiumid.sdk.fourf.VeridiumSDKFourFInitializer;
import com.veridiumid.sdk.model.exception.SDKInitializationException;

import com.veridiumid.sdk.IVeridiumSDK;
import com.veridiumid.sdk.VeridiumSDK;
import com.veridiumid.sdk.analytics.Analytics;
import com.veridiumid.sdk.fourf.ExportConfig;
import com.veridiumid.sdk.fourf.FourFInterface;
import com.veridiumid.sdk.fourf.FourFIntegrationWrapper;
import com.veridiumid.sdk.model.biometrics.packaging.IBiometricFormats;
import com.veridiumid.sdk.model.biometrics.results.BiometricResultsParser;
import com.veridiumid.sdk.model.biometrics.results.handling.IBiometricResultsHandler;
import com.veridiumid.sdk.licensing.exception.LicenseException;
import com.veridiumid.sdk.support.base.VeridiumBaseActivity;
import com.veridiumid.sdk.support.help.ToastHelper;
import insolutions.veridium.insolutionsveridiumsdk.ISVeridiumTracker;

import android.content.res.Resources;

import android.widget.Button;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

public class FourfingerActivity extends Activity {

    private IVeridiumSDK mBiometricSDK;

    // operation strings
    private static final int REQUEST_APP_PREF = 168;
    private static final int REQUEST_EXPORT = 314;
    private static final int REQUEST_ENROL = 324;
    private static final int REQUEST_AUTH = 334;

    private String package_name;
    private Resources resources;

    private AlertDialog dialog_permissions;

    private static final String[] requiredPermissions = new String[] {
            Manifest.permission.CAMERA,
            Manifest.permission.INTERNET
    };

    private static int RIGHT_THUMB = 1;
    private static int RIGHT_INDEX = 2;
    private static int RIGHT_MIDDLE = 3;
    private static int RIGHT_RING = 4;
    private static int RIGHT_PINKY = 5;

    private static int LEFT_THUMB = 6;
    private static int LEFT_INDEX = 7;
    private static int LEFT_MIDDLE = 8;
    private static int LEFT_RING = 9;
    private static int LEFT_PINKY = 10;

    private int BestFingerRight;
    private int BestFingerLeft;
    private boolean Liveness;

    private String TAG = "FourfingerActivity";
    private volatile boolean resultCompleted = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(getResourceId("layout/activity_fourfinger"));

        Log.d(TAG, "On create");

        BestFingerRight = getIntent().getIntExtra("RightFinger", 0);
        BestFingerLeft = getIntent().getIntExtra("LeftFinger", 0);
        Liveness = getIntent().getBooleanExtra("Liveness", false);

        Log.d(TAG, "intent got. Left " + String.valueOf(BestFingerLeft) + "Right " + String.valueOf(BestFingerRight));

        // Do not start a duplicate camera flow when Android recreates this activity.
        if (savedInstanceState != null) {
            finishWithError("ACTIVITY_RECREATED", null);
            return;
        }

        if (!preInitSDK() || !initSDK()) {
            finishWithError("SDK_NOT_INITIALIZED", null);
            return;
        }

        try {
            configureExportSettings();
        } catch (RuntimeException e) {
            finishWithError("EXPORT_CONFIGURATION_FAILED", e);
            return;
        }

        // Telemetry should never prevent a biometric result from returning to Cordova.
        try {
            Analytics.init(this);
        } catch (RuntimeException e) {
            Log.w(TAG, "Analytics initialization failed: " + e.getClass().getSimpleName());
        }

        if (Build.VERSION.SDK_INT > 22) {

            Log.d(TAG, "Requesting Permissions");
            checkPermissions(requiredPermissions);
        } else {

            abrir4Finger();
        }

    }

    @TargetApi(Build.VERSION_CODES.M)
    public void checkPermissions(@NonNull String... permissions) {

        List<String> ungranted = new ArrayList<String>();
        for (String permission : permissions) {
            if (ContextCompat.checkSelfPermission(this,
                    permission) != PackageManager.PERMISSION_GRANTED) {
                ungranted.add(permission);
            }
        }

        if (ungranted.size() != 0) {
            ActivityCompat.requestPermissions(this,
                    ungranted.toArray(new String[0]),
                    REQUEST_APP_PREF);
        } else {
            Log.d(TAG, "hasPermissions");
            abrir4Finger();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode != REQUEST_APP_PREF) {
            Log.w(TAG, "Unexpected permission requestCode=" + requestCode);
            return;
        }
        boolean hasAllPermissions = grantResults != null && grantResults.length > 0;
        if (hasAllPermissions) {
            for (int grant : grantResults) {
                if (grant != PackageManager.PERMISSION_GRANTED) {
                    hasAllPermissions = false;
                    break;
                }
            }
        }
        if (hasAllPermissions) {
            abrir4Finger();
        } else {
            showDeniedPermissionsDialog();
        }
    }

    private void showDeniedPermissionsDialog() {
        if (isFinishing() || resultCompleted) return;
        if (dialog_permissions != null && dialog_permissions.isShowing()) return;

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("Esta aplicación requiere permiso de cámara. Habilítelo en la configuración de Android.")
                .setCancelable(false)
                .setPositiveButton("OK", (dialog, id) -> finishWithError("CAMERA_PERMISSION_DENIED", null));
        dialog_permissions = builder.create();
        dialog_permissions.show();
    }

    private void abrir4Finger() {
        if (resultCompleted || isFinishing()) return;
        Log.d(TAG, "Opening 4F capture");
        if (mBiometricSDK == null) {
            finishWithError("SDK_NOT_INITIALIZED", null);
            return;
        }
        try {
            Intent exportIntent = mBiometricSDK.export(FourFInterface.UID);
            if (exportIntent == null) {
                finishWithError("START_CAPTURE_FAILED", null);
                return;
            }
            startActivityForResult(exportIntent, REQUEST_EXPORT);
        } catch (RuntimeException e) {
            finishWithError("START_CAPTURE_FAILED", e);
        }
    }

    private int getResourceId(String typeAndName) {
        if (package_name == null)
            package_name = getApplication().getPackageName();
        if (resources == null)
            resources = getApplication().getResources();
        return resources.getIdentifier(typeAndName, null, package_name);
    }

    /*
     * Initialise the SDK and ensure the licence is valid.
     */
    private boolean initSDK() {

        try {
            mBiometricSDK = VeridiumSDK.getSingleton();
            return mBiometricSDK != null;
        } catch (LicenseException e) {
            Log.e(TAG, "Veridium licence initialization failed", e);
        } catch (RuntimeException e) {
            Log.e(TAG, "Veridium SDK is unavailable", e);
        }
        return false;
    }

    /*
     * Configure Export settings using ExportConfig
     * Capture either hand, JSON container format, PNG with multiple scales,
     * liveness on
     */
    private void configureExportSettings() {

        ExportConfig.setFormat(IBiometricFormats.TemplateFormat.FORMAT_JSON);

        if (BestFingerRight == RIGHT_THUMB) {
            ExportConfig.setFingersToCapture(Arrays.asList(ExportConfig.FingerID.THUMB_RIGHT));

        } else if (BestFingerLeft == LEFT_THUMB) {
            ExportConfig.setFingersToCapture(Arrays.asList(ExportConfig.FingerID.THUMB_LEFT));

        } else if (BestFingerRight == RIGHT_INDEX || BestFingerRight == RIGHT_MIDDLE || BestFingerRight == RIGHT_RING
                || BestFingerRight == RIGHT_PINKY) {
            ExportConfig.setFingersToCapture(ExportConfig.ExportMode.FOUR_F_RIGHT_ENFORCED);

        } else if (BestFingerLeft == LEFT_INDEX || BestFingerLeft == LEFT_MIDDLE || BestFingerLeft == LEFT_RING
                || BestFingerLeft == LEFT_PINKY) {
            ExportConfig.setFingersToCapture(ExportConfig.ExportMode.FOUR_F_LEFT_ENFORCED);

        }

        /*
         * ExportConfig.setFormat(IBiometricFormats.TemplateFormat.FORMAT_JSON);
         * ExportConfig.setFixedPrintSize(512, 512);
         * ExportConfig.setWSQCompressionBitrate(0.5F);
         * ExportConfig.setPack_raw(false);
         * ExportConfig.setPack_wsq(true);
         * ExportConfig.setPack_png(false);
         * ExportConfig.setUseLiveness(true);
         * //ExportConfig.setLivenessFactor(99);
         * ExportConfig.setPackAuditImage(true);
         * ExportConfig.setPackDebugInfo(false);
         * ExportConfig.setUseNistType4(true);
         */

        // ExportConfig.setActiveLivenessBeta(false);
        //ExportConfig.setActiveLivenessBeta(Liveness);
        ExportConfig.setLivenessFactor(99);
        ExportConfig.setPack_bmp(false);
        ExportConfig.setPack_png(false);
        ExportConfig.setPack_raw(false);
        ExportConfig.setPack_wsq(true);
        ExportConfig.setPackExtraScale(true);
        ExportConfig.setFixedPrintSize(512, 512);
        ExportConfig.setUseLiveness(true);
        ExportConfig.setCalculate_NFIQ(true);
        ExportConfig.setUseNistType4(false);
    //-----    ExportConfig.setPackDebugInfo(true);
        ExportConfig.setPackAuditImage(true);
        ExportConfig.configureTimeout(true, 45, 3, true);
        /*
         * ExportConfig.setWSQCompressRatio(ExportConfig.WSQCompressRatio.COMPRESS_10to1
         * );
         * ExportConfig.setFixedPrintSize(0,0);
         */
    }

    /*
     * A custom IBiometricResultsHandler, handles the resulting data from an
     * operation
     */
    IBiometricResultsHandler resultHandler = new IBiometricResultsHandler() {
        @Override
        public void handleSuccess(Map<String, byte[][]> results) {
            if (resultCompleted) return;
            try {
                if (results == null || results.isEmpty()) {
                    finishWithError("EMPTY_BIOMETRIC_RESULTS", null);
                    safeTrackEvent("Captura", "Review");
                    return;
                }
                byte[][] templates = results.get(FourFInterface.UID);
                // Preserve existing vendor contract: exported JSON resides in index 1.
                if (templates == null || templates.length < 2 || templates[1] == null
                        || templates[1].length == 0) {
                    finishWithError("INVALID_BIOMETRIC_TEMPLATE", null);
                    safeTrackEvent("Capture", "Error");
                    return;
                }
                boolean parsed = ConvertByteArray(templates[1]);
                safeTrackEvent("Captura", parsed ? "Success" : "Review");
            } catch (RuntimeException e) {
                finishWithError("ERROR_BIOMETRIC_PROCESSING", e);
            }
        }

        @Override
        public void handleFailure() {
            finishWithError("Escaneo Fallido", null);
            safeTrackEvent("Capture", "Error");
        }

        @Override
        public void handleCancellation() {
            finishWithError("Escaneo Cancelado", null);
            safeTrackEvent("Capture", "Cancel");
        }

        @Override
        public void handleError(String message) {
            // Do not transmit SDK exception details / image data to telemetry.
            finishWithError("ERROR_BIOMETRIC_SDK", null);
            safeTrackEvent("Capture", "Error");
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "Veridium result: requestCode=" + requestCode + ", resultCode=" + resultCode
                + ", hasIntent=" + (data != null));
        try {
            if (requestCode == REQUEST_EXPORT || requestCode == REQUEST_ENROL || requestCode == REQUEST_AUTH) {
                BiometricResultsParser.parse(resultCode, data, resultHandler);
            } else if (requestCode != REQUEST_APP_PREF) {
                finishWithError("ERROR_UNEXPECTED_REQUEST", null);
            }
        } catch (RuntimeException e) {
            finishWithError("ERROR_RESULT_PARSER", e);
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private boolean ConvertByteArray(byte[] byteResponse) {
        try {
            if (byteResponse == null || byteResponse.length == 0) {
                finishWithError("INVALID_BIOMETRIC_TEMPLATE", null);
                return false;
            }
            JSONObject object = new JSONObject(new String(byteResponse, StandardCharsets.UTF_8));
            JSONObject scale085 = object.getJSONObject("SCALE085");
            JSONArray fingerprints = scale085.getJSONArray("Fingerprints");
            JSONObject scale100 = object.getJSONObject("SCALE100");
            JSONObject auditImage = scale100.getJSONObject("AuditImage_0");
            String imageBase64 = auditImage.getString("BinaryBase64ObjectJPG");

            if (fingerprints.length() == 0) {
                finishWithError("ERROR_EMPTY_FINGERPRINTS", null);
                return false;
            }

            int bestFingerIndex = -1;
            for (int index = 0; index < fingerprints.length(); index++) {
                JSONObject candidate = fingerprints.getJSONObject(index);
                int fingerCode = candidate.getInt("FingerPositionCode");
                if (fingerCode == BestFingerLeft || fingerCode == BestFingerRight) {
                    bestFingerIndex = index;
                    break;
                }
            }
            if (bestFingerIndex == -1) {
                finishWithError("FINGER_NOT_FOUND", null);
                return false;
            }

            JSONObject selected = fingerprints.getJSONObject(bestFingerIndex);
            int selectedFinger = selected.getInt("FingerPositionCode");
            JSONObject fingerImage = selected.getJSONObject("FingerImpressionImage");
            String hand;
            if (selectedFinger >= LEFT_THUMB && selectedFinger <= LEFT_PINKY) {
                hand = "LEFT";
            } else if (selectedFinger >= RIGHT_THUMB && selectedFinger <= RIGHT_PINKY) {
                hand = "RIGHT";
            } else {
                finishWithError("FINGER_NOT_FOUND", null);
                return false;
            }

            String wsqBase64 = fingerImage.getString("BinaryBase64ObjectWSQ");
            if (wsqBase64.isEmpty() || imageBase64.isEmpty()) {
                finishWithError("ERROR_EMPTY_TEMPLATE_DATA", null);
                return false;
            }

            // Return the same keys as the original plugin; no raw biometrics in logs.
            Intent result = new Intent();
            result.putExtra("base64String", wsqBase64);
            result.putExtra("hand", hand);
            result.putExtra("img", imageBase64);
            Log.d(TAG, "4F parsed successfully: hand=" + hand + ", wsqLength=" + wsqBase64.length()
                    + ", imgLength=" + imageBase64.length());
            if (wsqBase64.length() + imageBase64.length() > 400 * 1024) {
                Log.w(TAG, "Biometric result payload is large; Binder transaction size must be verified");
            }
            finishWithResult(Activity.RESULT_OK, result);
            return true;
        } catch (Exception e) {
            finishWithError("ERROR_PARSING_BIOMETRIC_RESULT", e);
            return false;
        }
    }

    private void finishWithError(String status, Throwable error) {
        if (error == null) {
            Log.e(TAG, "4F operation failed: " + status);
        } else {
            // Class name only: exception messages can contain protected biometric data.
            Log.e(TAG, "4F operation failed: " + status + ", cause="
                    + error.getClass().getSimpleName());
        }
        Intent intent = new Intent();
        intent.putExtra("status", status);
        finishWithResult(Activity.RESULT_CANCELED, intent);
    }

    private void finishWithResult(final int resultCode, final Intent data) {
        // Ensures setResult/finish run on UI thread even if Veridium calls back on a worker.
        runOnUiThread(() -> {
            if (resultCompleted || isFinishing()) return;
            resultCompleted = true;
            Log.d(TAG, "Completing 4F activity, resultCode=" + resultCode);
            try {
                setResult(resultCode, data);
                finish();
            } catch (RuntimeException e) {
                Log.e(TAG, "Unable to finish 4F activity: " + e.getClass().getSimpleName());
                try {
                    Intent fallback = new Intent();
                    fallback.putExtra("status", "ERROR_RESULT_TRANSFER");
                    setResult(Activity.RESULT_CANCELED, fallback);
                    finish();
                } catch (RuntimeException fallbackError) {
                    Log.e(TAG, "4F fallback finish failed: " + fallbackError.getClass().getSimpleName());
                }
            }
        });
    }

    private void safeTrackEvent(String action, String outcome) {
        try {
            ISVeridiumTracker.trackEvent(getApplicationContext(), "ENTEL AA", "Veridium", action, outcome);
        } catch (RuntimeException e) {
            Log.w(TAG, "Biometric telemetry failed: " + e.getClass().getSimpleName());
        }
    }

    private boolean preInitSDK() {

        Context appContext = getApplicationContext();

        try {
            // TODO add here the 4 FingersID Licence
            String fourfLicence = "C4WRPtrFB80QYIDMxffib+W2uHHaPCkzdV9ue2qqKRdzxinYxBspK4IYLsLw7qashM3nlpBAbPySdQLj7XY0AHsiZGV2aWNlRmluZ2VycHJpbnQiOiJKdGllNEtFMENVbDhWQnpVdWwva3VZcUdiTW5rMllPb2g0d2tLT09nV0JVPSIsImxpY2Vuc2UiOiJ4RSt1YnVrRk1aMVJFY0g1anRLTk1maWVBaGQyVHN5YXJVbTFFZlhlVUhGcFBQRjJsdVBvejJJM1VhUHpXcTFxS2dDTXFEbXN0L3NPVlM2MnJwMjlBbnNpZEhsd1pTSTZJa0pKVDB4SlFsTWlMQ0p1WVcxbElqb2lORVlpTENKc1lYTjBUVzlrYVdacFpXUWlPakUzTnpnMk9UZzRNekUxTkRVc0ltTnZiWEJoYm5sT1lXMWxJam9pU1c1emIyeDFkR2x2Ym5NZ0xTQkZiblJsYkNBeU1ESTNJaXdpWTI5dWRHRmpkRWx1Wm04aU9pSkJibTUxWVd3Z2JHbGpaVzV6WlNCbWIzSWdSVzUwWld3Z1pXNWthVzVuSUVwMWJtVWdNekFzSURJd01qY2dYRzVjYm1OdmJTNWxiblJsYkM1dGIzWnBiQzVoZFhSdllXTjBhWFpoWTJsdmJpeGNibU52YlM1bGJuUmxiQzV0YjNacGJDNWhjSEJrWld4cGRtVnllU3hjYm1OdmJTNXZkWFJ6ZVhOMFpXMXpaVzUwWlhKd2NtbHpaUzVsYm5SbGJDNUZiblJsYkZSbGNuSmxibThzWEc1amIyMHViM1YwYzNsemRHVnRjMlZ1ZEdWeWNISnBjMlV1Wlc1MFpXd3VVRVZOWVhsdmNtbHpkR0VnSWl3aVkyOXVkR0ZqZEVWdFlXbHNJam9pYW05dWJXRnlMbkJsY21WNlFHbHVjMjlzZFhScGIyNXpMbkJsSWl3aWMzVmlUR2xqWlc1emFXNW5VSFZpYkdsalMyVjVJam9pV0ZwNk1sZzVMMHhhTDBzM1REZHpNVGN3Wm1jelpWZENTWGxYZDJsMFZHWnZTMFJoZFd0NmVWUklWVDBpTENKemRHRnlkRVJoZEdVaU9qRTNOemcyTkRRNE1EQXdNREFzSW1WNGNHbHlZWFJwYjI1RVlYUmxJam94T0RFME16STRNREF3TURBd0xDSm5jbUZqWlVWdVpFUmhkR1VpT2pFNE1UVXhPVEl3TURBd01EQXNJblZ6YVc1blUwRk5URlJ2YTJWdUlqcG1ZV3h6WlN3aWRYTnBibWRHY21WbFVrRkVTVlZUSWpwbVlXeHpaU3dpZFhOcGJtZEJZM1JwZG1WRWFYSmxZM1J2Y25raU9tWmhiSE5sTENKaWFXOXNhV0pHWVdObFJYaHdiM0owUlc1aFlteGxaQ0k2Wm1Gc2MyVXNJbkoxYm5ScGJXVkZiblpwY205dWJXVnVkQ0k2ZXlKelpYSjJaWElpT21aaGJITmxMQ0prWlhacFkyVlVhV1ZrSWpwbVlXeHpaWDBzSW1abFlYUjFjbVZ6SWpwN0ltSmhjMlVpT25SeWRXVXNJbk4wWlhKbGIweHBkbVZ1WlhOeklqcDBjblZsTENKbGVIQnZjblFpT25SeWRXVjlMQ0psYm1admNtTmxaRkJ5WldabGNtVnVZMlZ6SWpwN0ltMWhibVJoZEc5eWVVeHBkbVZ1WlhOeklqcG1ZV3h6Wlgwc0luWmxjbk5wYjI0aU9pSXFMaW9pZlE9PSJ9";
            VeridiumSDK.init(appContext,
                    new DefaultVeridiumSDKModelFactory(appContext),
                    new VeridiumSDKFourFInitializer(fourfLicence),
                    new VeridiumSDKDataInitializer());
            // To get VeridiumVersion
            Log.e("VSDK: ", "" + VeridiumSDK.getSingleton().getVersionName());
            Log.e("VSDK4F:", "" + FourFIntegrationWrapper.version());
            return true;

        } catch (Exception e) {
            Log.e(TAG, "Veridium initialization failed: " + e.getClass().getSimpleName());
            return false;
        }

    }

    private static boolean tienePermisos(Context context, String... permisos) {
        if (context != null && permisos != null) {
            for (String permiso : permisos) {
                if (ActivityCompat.checkSelfPermission(context, permiso) != PackageManager.PERMISSION_GRANTED) {
                    return false;
                }
            }
        }
        return true;
    }

}
