# Security
TLS 1.3 Crypto

# Handshake Packets
Each Hello message must be sent in one packet. This may limit RSA size, though people usually don't use anything about RSA-4096 anyways, which should fit into 1200 bytes along with the rest of the data. 
Think about having asserts/checks to make sure payload size is under 1200 (or maybe 1000) bytes.
## Client Hello

| Value  | NV  | versions\<uint8\>[NV] | NC  | ciphers\<uint16\>\[NC\] |
| ------ | --- | --------------------- | --- | ----------------------- |
| Length | 1   | NV                    | 1   | 2 \* NC                 |

| Value  | CEPKT | CEPKL | CEPKV\<uint8\>\[CEPKL\] |
| ------ | ----- | ----- | ----------------------- |
| Length | 1     | 2     | CEPKL                   |

| Value  | CPKT | CPKL | CPKV\<uint8\>\[PKL\] |
| ------ | ---- | ---- | -------------------- |
| Length | 1    | 4    | PKL                  |

| Value  | HFL | HFV\<uint8\>\[HFL\] |
| ------ | --- | ------------------- |
| Length | 1   | HFL                 |

### Nonce

| Value  | Nonce |     |
| ------ | ----- | --- |
| Length | 32    |     |

**NV**: The number of protocol versions the client wishes to negotiate
**versions\<uint8\>\[NV\]**: The protocol versions which the client supports. Each value is uint8
**NC**: The number of ciphers the client wishes to negotiate. Set to 0 if you wish for unencrypted traffic.
**ciphers\<uint16\>\[NC\]**: The ciphers the client supports. Each value is uint16 and is an id representing the cipher number.
**CEPKT**: A uint8 representing the Client ephemeral public key type. Do not include if **NC** is 0.
**CEPKL**: A uint16 representing the Client ephemeral public key length. Do not include if **NC** is 0.
**CEPKV**: An array of uint8 representing the Client ephemeral public key value, with length **CEPKL**
**CPKT**: A uint8 representing the Client public key type. Set to 0 if unused.
**CPKL**: A uint32 representing the length of the Client public key. Do not add if unused.
**CPKV**: An array of uint8 representing the Client public key value, with length **CPKL**. Do not add if unused.
**HFL**: A uint8 representing the number of hash functions the client is willing to negotiate
**HFV\<uint8\>\[HFL\]**: The hash functions the Client supports. Each value is a uint8 representing the id of the hash function.
****

## Server Hello

| Value  | V   | C   | SEPKL | SEPKV\<uint8\>\[SEPKL\] | SPKT | SPKL | SPKV\<uint8\>\[SPKL\] |
| ------ | --- | --- | ----- | ----------------------- | ---- | ---- | --------------------- |
| Length | 1   | 2   | 1     | SEPKL                   | 1    | 4    | PKL                   |
**V**: The Protocol version number
**C**: The Cipher Suite id
**SEPKL**: The Length of the Server Ephemeral Key. Set to 0 if unused. If the received **CEPKL** is 0, this should be 0 or reject the connection.
**SEPKV**: The value of the server Ephemeral Key. Do not add if **EPKL** is 0.
**SPKT**: A uint8 representing the public key type. Set to 0 if unused.
**SPKL**: A uint32 representing the public key length. If **SPKT** is 0, do not add.
**SPKV**: An array of uint8 representing the public key value, with length **SPKL**. If **SPKT** is 0, do not add.
## The Cipher Suites and Values
There Cipher Suites are those allowed in TLS 1.3. A link to the cipher suites 

https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-28#page-131

| Description                  | Value       | Has Has Functino |
| ---------------------------- | ----------- | ---------------- |
| TLS_AES_128_GCM_SHA256       | {0x13,0x01} | Yes              |
| TLS_AES_256_GCM_SHA384       | {0x13,0x02} | Yes              |
| TLS_CHACHA20_POLY1305_SHA256 | {0x13,0x03} | Yes              |
| TLS_AES_128_CCM_SHA256       | {0x13,0x04} |                  |
| TLS_AES_128_CCM_8_SHA256     | {0x13,0x05} |                  |

## Public key Types and Values
### Ephemeral Keys
Max length of ephemeral key is 1024 bits.

| Key Type   | Value |
| ---------- | ----- |
| X25519     | 0x01  |
| P-256 ECDH | 0x02  |
| P-384 ECDH | 0x03  |
| FFDHE      | 0x04  |
### Identity Keys
Keys used to prove identity

| Key Type    | Value |
| ----------- | ----- |
| RSA         | 0x01  |
| P-256 ECDSA | 0x02  |
| P-384 ECDSA | 0x03  |
| Ed25519     | 0x04  |
## Hash Functions

| Hash Function | Value |
| ------------- | ----- |
| SHA-256       | 0x01  |
| SHA-384       | 0x02  |
| SHA-512       | 0x03  |
| RIPEMD-160    | 0x04  |
|               |       |


# Frames
These should all be encrypted before being sent out, unless specified otherwise.
## Create Stream Frame

| Value  | Frame Type = 0x1 | Stream ID | Stream Type |
| ------ | ---------------- | --------- | ----------- |
| Length | 1                | 8         | 1           |

| Stream Type                | Stream Type Value |
| -------------------------- | ----------------- |
| Reliable Sequenced         | 0x0               |
| Reliable Unsequenced       | 0x1               |
| Unreliable Sequenced       | 0x2               |
| Unreliable Unsequenced     | 0x3               |
| Latest Message Reliability | 0x4               |
[[Channel Parameters#Reliability#Unreliable]]
