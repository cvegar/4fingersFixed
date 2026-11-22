# Entel 4F Cordova Plugin — v0.0.3 (Android)

Versión de prueba para QA, derivada de v0.0.2. Se mantienen los AAR propietarios y el código iOS sin cambios.

## Cambios

- `plugin.xml`: ambas actividades propias de captura permanecen fijadas a orientación `portrait`; versionado sincronizado a 0.0.3.
- `FourfingerActivity.java`: finalización centralizada de `Activity`, errores con `status`, protección ante resultados nulos/vacíos, errores de JSON, permisos denegados, errores de SDK, problemas de inicialización y callback inesperado.
- `FourfingerActivity.java`: no selecciona la primera huella como respaldo cuando el `FingerPositionCode` no coincide; ahora devuelve `FINGER_NOT_FOUND`.
- `FourfingerActivity.java`: trazas de inicio, resultado y cierre sin contenidos biométricos; la telemetría se aísla con `try/catch`.
- `EntelFingerPlugin.java`: evita invocaciones simultáneas, valida extras/resultado, cierra el callback en todas las rutas controladas y deja de registrar WSQ/IMG Base64.
- Se conserva la interfaz JavaScript `getwsq(righFingerCode, leftFingerCode, liveness, success, error)` y el éxito `{wsq, hand, img}`.
- Para cancelación y fallo biométrico de usuario se sigue devolviendo `CANCEL`; para errores técnicos nuevos, el callback de error recibe un código específico (p.ej. `ERROR_PARSING_BIOMETRIC_RESULT`, `SDK_NOT_INITIALIZED`, `EMPTY_RESULT`). Validar este tratamiento en OutSystems.

## Límites y validaciones necesarias

1. **No compilado contra los AAR / SDK Android de Entel ni instalado en un dispositivo.** Requiere generar una nueva APK y probar en Motorola Android 15, además de otros equipos soportados.
2. `IBiometricResultsHandler` permanece por compatibilidad con los AAR integrados. Migrar a `IVBiometricResultsHandler` únicamente después de verificar la firma de API del proveedor.
3. Se sigue enviando imagen de auditoría `img` y WSQ a través de un `Intent`, para conservar el contrato. El log mostró una transacción Binder grande (~533 KB); aún existe riesgo de `TransactionTooLargeException`. Resolverlo bien requeriría acordar un transporte alternativo con OutSystems.
4. El error `GAv4 SecurityException` puede provenir de Analytics/Google y no se corrige modificando el plugin de cámara. Debe localizarse en la dependencia responsable.
5. `ExportConfig.setUseLiveness(true)` se conserva deliberadamente para no cambiar la validación biométrica; el parámetro JavaScript `liveness` sigue sin determinar esta opción. Requiere confirmación funcional antes de alterarlo.
6. Se mantienen las licencias incluidas por el proveedor. **No publicar este ZIP públicamente**. Revisar política de secretos y rotación de credenciales con el responsable.

## Pruebas recomendadas

- Captura exitosa: éxito exactamente una vez con `wsq`, `hand`, `img`; regreso inmediato a Cordova y sin rotación al girar.
- Cancelación: error `CANCEL`, regreso a OutSystems y posibilidad de un nuevo escaneo.
- Denegación de cámara: `CAMERA_PERMISSION_DENIED` después de pulsar OK; Activity cerrada.
- JSON malformado / huellas vacías / dedo no encontrado: error técnico explícito y Activity cerrada; verificar que no quede pantalla negra.
- Repetir `getwsq` mientras hay un escaneo: `SCAN_IN_PROGRESS` en segunda llamada, la primera completa su callback.
- Revisar Logcat filtrando `FourfingerActivity` y `EntelFingerPlugin-Cordova` sin exponer templates.
