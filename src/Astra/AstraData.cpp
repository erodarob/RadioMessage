
// This is the implementation file for the ASTRAData class defined within ASTRAData.h
// This class will be used ot carry filtered output post state estimation for HITL purposes
// This class holds FILTERED / FUSED outputs (State = Mahony attitude + Linear KF translation)
// Used for polymorphic data transmission through RadioMessage

#include "AstraData.h"

AstraData::AstraData()
    : timestamp_s(0.0f),
      px(0.0f), py(0.0f), pz(0.0f),
      vx(0.0f), vy(0.0f), vz(0.0f),
      ax_e(0.0f), ay_e(0.0f), az_e(0.0f),
      qw(1.0f), qx(0.0f), qy(0.0f), qz(0.0f)
{
} // default constructor

AstraData::AstraData(float _timestamp_s,
                     float _px, float _py, float _pz,
                     float _vx, float _vy, float _vz,
                     float _ax_e, float _ay_e, float _az_e,
                     float _qw, float _qx, float _qy, float _qz)
    : timestamp_s(_timestamp_s),
      px(_px), py(_py), pz(_pz),
      vx(_vx), vy(_vy), vz(_vz),
      ax_e(_ax_e), ay_e(_ay_e), az_e(_az_e),
      qw(_qw), qx(_qx), qy(_qy), qz(_qz)
{
} // parameterized constructor

int AstraData::encode(uint8_t *data, uint16_t sz)
{
    if (data == nullptr)
        return 0;

    uint16_t pos = 0;
    const uint16_t totalSize = static_cast<uint16_t>(PAYLOAD_SIZE);

    if (sz < totalSize)
        return 0; // provided buffer too small

    memcpy(&data[pos], &timestamp_s, sizeof(timestamp_s));
    pos += sizeof(timestamp_s);

    // position
    memcpy(&data[pos], &px, sizeof(px));
    pos += sizeof(px);
    memcpy(&data[pos], &py, sizeof(py));
    pos += sizeof(py);
    memcpy(&data[pos], &pz, sizeof(pz));
    pos += sizeof(pz);

    // velocity
    memcpy(&data[pos], &vx, sizeof(vx));
    pos += sizeof(vx);
    memcpy(&data[pos], &vy, sizeof(vy));
    pos += sizeof(vy);
    memcpy(&data[pos], &vz, sizeof(vz));
    pos += sizeof(vz);

    // earth-frame acceleration (control input used in KF predict)
    memcpy(&data[pos], &ax_e, sizeof(ax_e));
    pos += sizeof(ax_e);
    memcpy(&data[pos], &ay_e, sizeof(ay_e));
    pos += sizeof(ay_e);
    memcpy(&data[pos], &az_e, sizeof(az_e));
    pos += sizeof(az_e);

    // quaternion
    memcpy(&data[pos], &qw, sizeof(qw));
    pos += sizeof(qw);
    memcpy(&data[pos], &qx, sizeof(qx));
    pos += sizeof(qx);
    memcpy(&data[pos], &qy, sizeof(qy));
    pos += sizeof(qy);
    memcpy(&data[pos], &qz, sizeof(qz));
    pos += sizeof(qz);

    if (pos != totalSize)
        return 0; // encoding error

    return pos;
}

int AstraData::decode(uint8_t *data, uint16_t sz)
{
    if (data == nullptr)
        return 0;

    uint16_t pos = 0;
    const uint16_t totalSize = static_cast<uint16_t>(PAYLOAD_SIZE);

    if (sz < totalSize)
        return 0; // provided buffer too small

    memcpy(&timestamp_s, &data[pos], sizeof(timestamp_s));
    pos += sizeof(timestamp_s);

    // position
    memcpy(&px, &data[pos], sizeof(px));
    pos += sizeof(px);
    memcpy(&py, &data[pos], sizeof(py));
    pos += sizeof(py);
    memcpy(&pz, &data[pos], sizeof(pz));
    pos += sizeof(pz);

    // velocity
    memcpy(&vx, &data[pos], sizeof(vx));
    pos += sizeof(vx);
    memcpy(&vy, &data[pos], sizeof(vy));
    pos += sizeof(vy);
    memcpy(&vz, &data[pos], sizeof(vz));
    pos += sizeof(vz);

    // earth-frame acceleration
    memcpy(&ax_e, &data[pos], sizeof(ax_e));
    pos += sizeof(ax_e);
    memcpy(&ay_e, &data[pos], sizeof(ay_e));
    pos += sizeof(ay_e);
    memcpy(&az_e, &data[pos], sizeof(az_e));
    pos += sizeof(az_e);

    // quaternion
    memcpy(&qw, &data[pos], sizeof(qw));
    pos += sizeof(qw);
    memcpy(&qx, &data[pos], sizeof(qx));
    pos += sizeof(qx);
    memcpy(&qy, &data[pos], sizeof(qy));
    pos += sizeof(qy);
    memcpy(&qz, &data[pos], sizeof(qz));
    pos += sizeof(qz);

    if (pos != totalSize)
        return 0; // decoding error

    return pos;
}

int AstraData::toJSON(char *json, uint16_t sz, int deviceId)
{
    if (json == nullptr)
        return 0;

    uint16_t result = (uint16_t)snprintf(
        json,
        sz,
        "{"
        "\"type\":\"ASTRAData\","
        "\"deviceId\":%d,"
        "\"data\":{"
        "\"t\":%.7f,"
        "\"px\":%.7f,\"py\":%.7f,\"pz\":%.7f,"
        "\"vx\":%.7f,\"vy\":%.7f,\"vz\":%.7f,"
        "\"ax_e\":%.7f,\"ay_e\":%.7f,\"az_e\":%.7f,"
        "\"qw\":%.7f,\"qx\":%.7f,\"qy\":%.7f,\"qz\":%.7f"
        "}"
        "}",
        deviceId,
        timestamp_s,
        px, py, pz,
        vx, vy, vz,
        ax_e, ay_e, az_e,
        qw, qx, qy, qz);

    if (result < sz)
        return result;

    return 0; // output too large
}

int AstraData::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    if (json == nullptr)
        return 0;

    // ---- buffers for extracted strings ----
    char deviceIdStr[8] = {0};
    char tStr[18] = {0};

    char pxStr[18] = {0};
    char pyStr[18] = {0};
    char pzStr[18] = {0};

    char vxStr[18] = {0};
    char vyStr[18] = {0};
    char vzStr[18] = {0};

    char axStr[18] = {0};
    char ayStr[18] = {0};
    char azStr[18] = {0};

    char qwStr[18] = {0};
    char qxStr[18] = {0};
    char qyStr[18] = {0};
    char qzStr[18] = {0};

    // ---- extract fields ----
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr, sizeof(deviceIdStr)))
        return 0;

    if (!extractStr(json, sz, "\"t\":", ',', tStr, sizeof(tStr)))
        return 0;

    if (!extractStr(json, sz, "\"px\":", ',', pxStr, sizeof(pxStr)))
        return 0;
    if (!extractStr(json, sz, "\"py\":", ',', pyStr, sizeof(pyStr)))
        return 0;
    if (!extractStr(json, sz, "\"pz\":", ',', pzStr, sizeof(pzStr)))
        return 0;

    if (!extractStr(json, sz, "\"vx\":", ',', vxStr, sizeof(vxStr)))
        return 0;
    if (!extractStr(json, sz, "\"vy\":", ',', vyStr, sizeof(vyStr)))
        return 0;
    if (!extractStr(json, sz, "\"vz\":", ',', vzStr, sizeof(vzStr)))
        return 0;

    if (!extractStr(json, sz, "\"ax_e\":", ',', axStr, sizeof(axStr)))
        return 0;
    if (!extractStr(json, sz, "\"ay_e\":", ',', ayStr, sizeof(ayStr)))
        return 0;
    if (!extractStr(json, sz, "\"az_e\":", ',', azStr, sizeof(azStr)))
        return 0;

    if (!extractStr(json, sz, "\"qw\":", ',', qwStr, sizeof(qwStr)))
        return 0;
    if (!extractStr(json, sz, "\"qx\":", ',', qxStr, sizeof(qxStr)))
        return 0;
    if (!extractStr(json, sz, "\"qy\":", ',', qyStr, sizeof(qyStr)))
        return 0;

    // last field ends with '}'
    if (!extractStr(json, sz, "\"qz\":", '}', qzStr, sizeof(qzStr)))
        return 0;

    // ---- convert to numeric types ----
    deviceId = atoi(deviceIdStr);

    timestamp_s = (float)atof(tStr);

    px = (float)atof(pxStr);
    py = (float)atof(pyStr);
    pz = (float)atof(pzStr);

    vx = (float)atof(vxStr);
    vy = (float)atof(vyStr);
    vz = (float)atof(vzStr);

    ax_e = (float)atof(axStr);
    ay_e = (float)atof(ayStr);
    az_e = (float)atof(azStr);

    qw = (float)atof(qwStr);
    qx = (float)atof(qxStr);
    qy = (float)atof(qyStr);
    qz = (float)atof(qzStr);

    // Keep your convention: return 1 on success (like your HITLData::fromJSON)
    return 1;
}