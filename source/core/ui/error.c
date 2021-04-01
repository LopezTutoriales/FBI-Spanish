#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <3ds.h>
#include <curl/curl.h>

#include "error.h"
#include "prompt.h"
#include "../error.h"
#include "../screen.h"
#include "../../fbi/resources.h"

static const char* level_to_string(Result res) {
    switch(R_LEVEL(res)) {
        case RL_SUCCESS:
            return "Exito";
        case RL_INFO:
            return "Informacion";
        case RL_FATAL:
            return "Fatal";
        case RL_RESET:
            return "Reiniciar";
        case RL_REINITIALIZE:
            return "Reinicializar";
        case RL_USAGE:
            return "Uso";
        case RL_PERMANENT:
            return "Permanente";
        case RL_TEMPORARY:
            return "Temporal";
        case RL_STATUS:
            return "Estado";
        default:
            return "<Desconocido>";
    }
}

static const char* summary_to_string(Result res) {
    switch(R_SUMMARY(res)) {
        case RS_SUCCESS:
            return "Exito";
        case RS_NOP:
            return "No";
        case RS_WOULDBLOCK:
            return "Bloquearia";
        case RS_OUTOFRESOURCE:
            return "Sin recursos";
        case RS_NOTFOUND:
            return "No encontrado";
        case RS_INVALIDSTATE:
            return "Estado no valido";
        case RS_NOTSUPPORTED:
            return "No soportado";
        case RS_INVALIDARG:
            return "Argumento no valido";
        case RS_WRONGARG:
            return "Argumento equivocado";
        case RS_CANCELED:
            return "Cancelado";
        case RS_STATUSCHANGED:
            return "Estado cambiado";
        case RS_INTERNAL:
            return "Interno";
        default:
            return "<Desconocido>";
    }
}

static const char* module_to_string(Result res) {
    switch(R_MODULE(res)) {
        case RM_COMMON:
            return "Comun";
        case RM_KERNEL:
            return "Kernel";
        case RM_UTIL:
            return "Util";
        case RM_FILE_SERVER:
            return "Servidor de archivos";
        case RM_LOADER_SERVER:
            return "Servidor cargador";
        case RM_TCB:
            return "TCB";
        case RM_OS:
            return "OS";
        case RM_DBG:
            return "DBG";
        case RM_DMNT:
            return "DMNT";
        case RM_PDN:
            return "PDN";
        case RM_GSP:
            return "GSP";
        case RM_I2C:
            return "I2C";
        case RM_GPIO:
            return "GPIO";
        case RM_DD:
            return "DD";
        case RM_CODEC:
            return "CODEC";
        case RM_SPI:
            return "SPI";
        case RM_PXI:
            return "PXI";
        case RM_FS:
            return "FS";
        case RM_DI:
            return "DI";
        case RM_HID:
            return "HID";
        case RM_CAM:
            return "CAM";
        case RM_PI:
            return "PI";
        case RM_PM:
            return "PM";
        case RM_PM_LOW:
            return "PMLOW";
        case RM_FSI:
            return "FSI";
        case RM_SRV:
            return "SRV";
        case RM_NDM:
            return "NDM";
        case RM_NWM:
            return "NWM";
        case RM_SOC:
            return "SOC";
        case RM_LDR:
            return "LDR";
        case RM_ACC:
            return "ACC";
        case RM_ROMFS:
            return "RomFS";
        case RM_AM:
            return "AM";
        case RM_HIO:
            return "HIO";
        case RM_UPDATER:
            return "Actualizador";
        case RM_MIC:
            return "MIC";
        case RM_FND:
            return "FND";
        case RM_MP:
            return "MP";
        case RM_MPWL:
            return "MPWL";
        case RM_AC:
            return "AC";
        case RM_HTTP:
            return "HTTP";
        case RM_DSP:
            return "DSP";
        case RM_SND:
            return "SND";
        case RM_DLP:
            return "DLP";
        case RM_HIO_LOW:
            return "HIOLOW";
        case RM_CSND:
            return "CSND";
        case RM_SSL:
            return "SSL";
        case RM_AM_LOW:
            return "AMLOW";
        case RM_NEX:
            return "NEX";
        case RM_FRIENDS:
            return "Amigos";
        case RM_RDT:
            return "RDT";
        case RM_APPLET:
            return "Applet";
        case RM_NIM:
            return "NIM";
        case RM_PTM:
            return "PTM";
        case RM_MIDI:
            return "MIDI";
        case RM_MC:
            return "MC";
        case RM_SWC:
            return "SWC";
        case RM_FATFS:
            return "FatFS";
        case RM_NGC:
            return "NGC";
        case RM_CARD:
            return "Tarjeta";
        case RM_CARDNOR:
            return "CARDNOR";
        case RM_SDMC:
            return "SDMC";
        case RM_BOSS:
            return "Jefe";
        case RM_DBM:
            return "DBM";
        case RM_CONFIG:
            return "Configuracion";
        case RM_PS:
            return "PS";
        case RM_CEC:
            return "CEC";
        case RM_IR:
            return "IR";
        case RM_UDS:
            return "UDS";
        case RM_PL:
            return "PL";
        case RM_CUP:
            return "CUP";
        case RM_GYROSCOPE:
            return "Giroscopio";
        case RM_MCU:
            return "MCU";
        case RM_NS:
            return "NS";
        case RM_NEWS:
            return "Noticias";
        case RM_RO:
            return "RO";
        case RM_GD:
            return "GD";
        case RM_CARD_SPI:
            return "CARDSPI";
        case RM_EC:
            return "EC";
        case RM_WEB_BROWSER:
            return "Navegador";
        case RM_TEST:
            return "TEST";
        case RM_ENC:
            return "ENC";
        case RM_PIA:
            return "PIA";
        case RM_ACT:
            return "ACT";
        case RM_VCTL:
            return "VCTL";
        case RM_OLV:
            return "OLV";
        case RM_NEIA:
            return "NEIA";
        case RM_NPNS:
            return "NPNS";
        case RM_AVD:
            return "AVD";
        case RM_L2B:
            return "L2B";
        case RM_MVD:
            return "MVD";
        case RM_NFC:
            return "NFC";
        case RM_UART:
            return "UART";
        case RM_SPM:
            return "SPM";
        case RM_QTM:
            return "QTM";
        case RM_NFP:
            return "NFP";
        case RM_APPLICATION:
            return "Aplicacion";
        default:
            return "<Desconocido>";
    }
}

static const char* description_to_string(Result res) {
    int module = R_MODULE(res);
    int description = R_DESCRIPTION(res);

    switch(module) {
        case RM_KERNEL:
            switch(description) {
                case 2:
                    return "Permisos de memoria buffer DMA no validos";
                default:
                    break;
            }

            break;
        case RM_OS:
            switch(description) {
                case 1:
                    return "Objeto fuera de sincronizacion";
                case 2:
                    return "Fuera de la memoria compartida de objetos";
                case 9:
                    return "Objetos fuera de sesion";
                case 10:
                    return "Sin memoria para la asignacion";
                case 20:
                    return "Permisos incorrectos para el acceso sin privilegios";
                case 26:
                    return "Sesion cerrada por proceso remoto";
                case 47:
                    return "Encabezado de comando no valido";
                case 52:
                    return "Maximo de conexiones de puerto excedidas";
                default:
                    break;
            }

            break;
        case RM_FS:
            switch(description) {
                case 101:
                    return "Archivo no montado";
                case 120:
                    return "No existe / Fallo al abrir";
                case 141:
                    return "Tarjeta de juego no insertada";
                case 171:
                    return "Bus: Ocupado / Sin datos";
                case 172:
                    return "Bus: Funcion ilegal";
                case 190:
                    return "Ya existe / Fallo al crear";
                case 210:
                    return "Tarjeta de memoria llena";
                case 230:
                    return "Operacion ilegal / Archivo en uso";
                case 231:
                    return "Recurso bloqueado";
                case 250:
                    return "Operacion FAT denegada";
                case 265:
                    return "Bus: Se acabo el tiempo";
                case 331:
                    return "Error de bus / NAND Nintendo DSi (TWL) no valida";
                case 332:
                    return "Bus: Error de bit de parada";
                case 391:
                    return "Fallo de verificacion de hash";
                case 392:
                    return "Fallo de verificacion de RSA/Hash";
                case 395:
                    return "Archivos de rom no validos o bloque de datos guardados";
                case 630:
                    return "Permiso de archivo denegado";
                case 702:
                    return "Ruta no valida / Archivo inaccesible";
                case 705:
                    return "Desplazamiento fuera del limite";
                case 721:
                    return "Limite de tamaño de archivo alcanzado";
                case 760:
                    return "Operacion no soportada";
                case 761:
                    return "Discrepancia en lectura de Archivos de ejecucion";
                default:
                    break;
            }

            break;
        case RM_SRV:
            switch(description) {
                case 5:
                    return "Longitud de nombre del servicio no valido";
                case 6:
                    return "Acceso denegado al servicio";
                case 7:
                    return "Discrepancia de tamaño del nombre";
                default:
                    break;
            }

            break;
        case RM_AM:
            switch(description) {
                case 4:
                    return "Estado de instalacion incorrecto";
                case 37:
                    return "NCCH no valido";
                case 39:
                    return "Version de titulo no valida o desactualizado";
                case 41:
                    return "Error de tipo 1";
                case 43:
                    return "La base de datos no existe";
                case 44:
                    return "Intento borrar un titulo de sistema";
                case 101:
                    return "Error de tipo -1";
                case 102:
                    return "Error de tipo -2";
                case 103:
                    return "Error de tipo -3";
                case 104:
                    return "Error de tipo -4";
                case 105:
                    return "Error de tipo -5";
                case 106:
                    return "Fallo de certificado o hash";
                case 107:
                    return "Error de tipo -7";
                case 108:
                    return "Error de tipo -8";
                case 109:
                    return "Error de tipo -9";
                case 110:
                    return "Error de tipo -10";
                case 111:
                    return "Error de tipo -11";
                case 112:
                    return "Error de tipo -12";
                case 113:
                    return "Error de tipo -13";
                case 114:
                    return "Error de tipo -14";
                case 393:
                    return "Base de datos no valida";
                default:
                    break;
            }

            break;
        case RM_HTTP:
            switch(description) {
                case 60:
                    return "Fallo al verificar el certificado TLS";
                case 70:
                    return "Red no disponible";
                case 102:
                    return "Identificador de contexto incorrecto";
                case 105:
                    return "Tiempo de espera agotado";
                default:
                    break;
            }

            break;
        case RM_SSL:
            switch(description) {
                case 20:
                    return "Certificado no es de confianza";
                case 54:
                    return "No se encontro el identificador de RootCertChain";
                default:
                    break;
            }

            break;
        case RM_SDMC:
            switch(description) {
                case 1:
                    return "Bus: Error de Bit23";
                case 2:
                    return "Bus: Error de RX listo";
                case 3:
                    return "Bus: Error de Bit28";
                case 4:
                    return "Bus: Error de Bit27";
                default:
                    break;
            }

            break;
        case RM_MVD:
            switch(description) {
                case 271:
                    return "Configuracion no valida";
                default:
                    break;
            }

            break;
        case RM_NFC:
            switch(description) {
                case 512:
                    return "Estado del NFC no valido";
                default:
                    break;
            }

            break;
        case RM_QTM:
            switch(description) {
                case 8:
                    return "Camara ocupada";
                default:
                    break;
            }

            break;
        case RM_APPLICATION:
            switch(res) {
                case R_APP_INVALID_ARGUMENT:
                    return "Argumento no valido";
                case R_APP_CANCELLED:
                    return "Operacion cancelada";
                case R_APP_SKIPPED:
                    return "Operacion omitida";
                case R_APP_THREAD_CREATE_FAILED:
                    return "Error al crear hilo";
                case R_APP_PARSE_FAILED:
                    return "Fallo al parsear";
                case R_APP_BAD_DATA:
                    return "Datos malos";
                case R_APP_HTTP_TOO_MANY_REDIRECTS:
                    return "Demadiados redireccionamientos";
                default:
                    if(res >= R_APP_HTTP_ERROR_BASE && res < R_APP_HTTP_ERROR_END) {
                        switch(res - R_APP_HTTP_ERROR_BASE) {
                            case 100:
                                return "HTTP 100: Continuar";
                            case 101:
                                return "HTTP 101: Protocolos de commutacion";
                            case 102:
                                return "HTTP 102: Procesando";
                            case 103:
                                return "HTTP 103: Early Hints";
                            case 200:
                                return "HTTP 200: OK";
                            case 201:
                                return "HTTP 201: Creado";
                            case 202:
                                return "HTTP 202: Aceptado";
                            case 203:
                                return "HTTP 203: Informacion no autorizada";
                            case 204:
                                return "HTTP 204: Sin contenido";
                            case 205:
                                return "HTTP 205: Restablecer contenido";
                            case 206:
                                return "HTTP 206: Contenido parcial";
                            case 207:
                                return "HTTP 207: Multi-estado";
                            case 208:
                                return "HTTP 208: Ya informado";
                            case 226:
                                return "HTTP 226: Estoy usado";
                            case 300:
                                return "HTTP 300: Multiples opciones";
                            case 301:
                                return "HTTP 301: Movido permanentemente";
                            case 302:
                                return "HTTP 302: Encontrado";
                            case 303:
                                return "HTTP 303: Ver otros";
                            case 304:
                                return "HTTP 304: No modificado";
                            case 305:
                                return "HTTP 305: Usar Proxy";
                            case 306:
                                return "HTTP 306: Cambiar proxy";
                            case 307:
                                return "HTTP 307: Redireccionamiento temporal";
                            case 308:
                                return "HTTP 308: Redireccionamiento permanente";
                            case 400:
                                return "HTTP 400: Solicitud incorrecta";
                            case 401:
                                return "HTTP 401: No autorizado";
                            case 402:
                                return "HTTP 402: Pago requerido";
                            case 403:
                                return "HTTP 403: Prohibido";
                            case 404:
                                return "HTTP 404: No encontrado";
                            case 405:
                                return "HTTP 405: Metodo no permitido";
                            case 406:
                                return "HTTP 406: Inaceptable";
                            case 407:
                                return "HTTP 407: Se requiere autenticacion proxy";
                            case 408:
                                return "HTTP 408: Tiempo de peticion acabado";
                            case 409:
                                return "HTTP 409: Conflicto";
                            case 410:
                                return "HTTP 410: Desaparecido";
                            case 411:
                                return "HTTP 411: Longitud requerida";
                            case 412:
                                return "HTTP 412: Fallo condicion previa";
                            case 413:
                                return "HTTP 413: Payload demasiado grande";
                            case 414:
                                return "HTTP 414: URI demasiado larga";
                            case 415:
                                return "HTTP 415: Tipo de medio no soportado";
                            case 416:
                                return "HTTP 416: Rango no satisfactorio";
                            case 417:
                                return "HTTP 417: Expectativa fallida";
                            case 418:
                                return "HTTP 418: Soy una tetera";
                            case 421:
                                return "HTTP 421: Solicitud mal dirigida";
                            case 422:
                                return "HTTP 422: Entidad no procesable";
                            case 423:
                                return "HTTP 423: Bloqueado";
                            case 424:
                                return "HTTP 424: Dependencia fallida";
                            case 426:
                                return "HTTP 426: Se requiere actualizacion";
                            case 428:
                                return "HTTP 428: Requisito previo";
                            case 429:
                                return "HTTP 429: Demasiadas solicitudes";
                            case 431:
                                return "HTTP 431: Solicitud de encabezados demasiado grandes";
                            case 451:
                                return "HTTP 451: No disponible por motivos legales";
                            case 500:
                                return "HTTP 500: Error interno del servidor";
                            case 501:
                                return "HTTP 501: No implementado";
                            case 502:
                                return "HTTP 502: Puerta de enlace incorrecta";
                            case 503:
                                return "HTTP 503: Servicio no disponible";
                            case 504:
                                return "HTTP 504: Tiempo agotado de puerta de enlace";
                            case 505:
                                return "HTTP 505: Version de HTTP no especificada";
                            case 506:
                                return "HTTP 506: La variantes tambien negocia";
                            case 507:
                                return "HTTP 507: Espacio insuficiente";
                            case 508:
                                return "HTTP 508: Bucle detectado";
                            case 510:
                                return "HTTP 510: No extendido";
                            case 511:
                                return "HTTP 511: Se requiere autenticacion de red";
                            default:
                                return "HTTP: Codigo de respuesta desconocido";
                        }
                    } else if(res >= R_APP_CURL_ERROR_BASE && res < R_APP_CURL_ERROR_END) {
                        return curl_easy_strerror(res - R_APP_CURL_ERROR_BASE);
                    }

                    break;
            }
        default:
            break;
    }

    switch(description) {
        case RD_SUCCESS:
            return "Exito";
        case RD_TIMEOUT:
            return "Se acabo el tiempo";
        case RD_OUT_OF_RANGE:
            return "Fuera de rango";
        case RD_ALREADY_EXISTS:
            return "Ya existe";
        case RD_CANCEL_REQUESTED:
            return "Cancelar solicitado";
        case RD_NOT_FOUND:
            return "No encontrado";
        case RD_ALREADY_INITIALIZED:
            return "Ya inicializado";
        case RD_NOT_INITIALIZED:
            return "No inicializado";
        case RD_INVALID_HANDLE:
            return "Identificador no valido";
        case RD_INVALID_POINTER:
            return "Puntero no valido";
        case RD_INVALID_ADDRESS:
            return "Direccion no valida";
        case RD_NOT_IMPLEMENTED:
            return "No se ha implementado";
        case RD_OUT_OF_MEMORY:
            return "Memoria insuficiente";
        case RD_MISALIGNED_SIZE:
            return "Tamaño desalineado";
        case RD_MISALIGNED_ADDRESS:
            return "Direccion desalineada";
        case RD_BUSY:
            return "Ocupado";
        case RD_NO_DATA:
            return "Sin datos";
        case RD_INVALID_COMBINATION:
            return "Combinacion no valida";
        case RD_INVALID_ENUM_VALUE:
            return "Valor de enumaracion no valido";
        case RD_INVALID_SIZE:
            return "Tamaño no valido";
        case RD_ALREADY_DONE:
            return "Ya hecho";
        case RD_NOT_AUTHORIZED:
            return "No autorizado";
        case RD_TOO_LARGE:
            return "Demasiado grande";
        case RD_INVALID_SELECTION:
            return "Seleccion no valida";
        default:
            return "<Desconocido>";
    }
}

typedef struct {
    char fullText[4096];
    void* data;
    void (*drawTop)(ui_view* view, void* data, float x1, float y1, float x2, float y2);
} error_data;

static void error_draw_top(ui_view* view, void* data, float x1, float y1, float x2, float y2) {
    error_data* errorData = (error_data*) data;

    if(errorData->drawTop != NULL) {
        errorData->drawTop(view, errorData->data, x1, y1, x2, y2);
    }
}

static void error_onresponse(ui_view* view, void* data, u32 response) {
    free(data);
}

ui_view* error_display(void* data, void (*drawTop)(ui_view* view, void* data, float x1, float y1, float x2, float y2), const char* text, ...) {
    error_data* errorData = (error_data*) calloc(1, sizeof(error_data));
    if(errorData == NULL) {
        // No use trying to spawn another if we're out of memory.
        return NULL;
    }

    errorData->data = data;
    errorData->drawTop = drawTop;

    va_list list;
    va_start(list, text);
    vsnprintf(errorData->fullText, 4096, text, list);
    va_end(list);

    return prompt_display_notify("Error", errorData->fullText, COLOR_TEXT, errorData, error_draw_top, error_onresponse);
}

ui_view* error_display_res(void* data, void (*drawTop)(ui_view* view, void* data, float x1, float y1, float x2, float y2), Result result, const char* text, ...) {
    error_data* errorData = (error_data*) calloc(1, sizeof(error_data));
    if(errorData == NULL) {
        // No use trying to spawn another if we're out of memory.
        return NULL;
    }

    errorData->data = data;
    errorData->drawTop = drawTop;

    char textBuf[1024];
    va_list list;
    va_start(list, text);
    vsnprintf(textBuf, 1024, text, list);
    va_end(list);

    int level = R_LEVEL(result);
    int summary = R_SUMMARY(result);
    int module = R_MODULE(result);
    int description = R_DESCRIPTION(result);

    snprintf(errorData->fullText, 4096, "%s\nCodigo: 0x%08lX\nNivel: %s (%d)\nResumen: %s (%d)\nModulo: %s (%d)\nDetalles: %s (%d)", textBuf, result, level_to_string(result), level, summary_to_string(result), summary, module_to_string(result), module, description_to_string(result), description);

    return prompt_display_notify("Error", errorData->fullText, COLOR_TEXT, errorData, error_draw_top, error_onresponse);
}

ui_view* error_display_errno(void* data, void (*drawTop)(ui_view* view, void* data, float x1, float y1, float x2, float y2), int err, const char* text, ...) {
    error_data* errorData = (error_data*) calloc(1, sizeof(error_data));
    if(errorData == NULL) {
        // No use trying to spawn another if we're out of memory.
        return NULL;
    }

    errorData->data = data;
    errorData->drawTop = drawTop;

    char textBuf[1024];
    va_list list;
    va_start(list, text);
    vsnprintf(textBuf, 1024, text, list);
    va_end(list);

    if(err < 0) {
        err = -err;
    }

    snprintf(errorData->fullText, 4096, "%s\nI/O Error: %s (%d)", textBuf, strerror(err), err);

    return prompt_display_notify("Error", errorData->fullText, COLOR_TEXT, errorData, error_draw_top, error_onresponse);
}