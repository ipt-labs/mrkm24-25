from fastapi import FastAPI, File, UploadFile
from defs import generate_keys, sign_document, verify_signature

app = FastAPI()


@app.post("/generate_keys")
def generate_keys_endpoint():
    private_key, public_key = generate_keys()
    return {"private_key": private_key.decode(), "public_key": public_key.decode()}


@app.post("/sign_document")
def sign_document_endpoint(file: UploadFile, private_key: str):
    private_key = private_key.replace("\\n", "\n")
    document = file.file.read()
    signature = sign_document(document, private_key.encode())
    return {"signature": signature.hex()}


@app.post("/verify_signature")
def verify_signature_endpoint(file: UploadFile, signature: str, public_key: str):
    public_key = public_key.replace("\\n", "\n")
    document = file.file.read()
    is_valid = verify_signature(document, bytes.fromhex(signature), public_key.encode())
    return {"is_valid": is_valid}

