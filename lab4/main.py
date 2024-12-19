from flask import Flask, request, jsonify, send_file
from werkzeug.utils import secure_filename
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
import os

app = Flask(__name__)


# Завантаження приватного ключа
def load_private_key(pem_file):
    with open(pem_file, 'rb') as key_file:
        private_key = serialization.load_pem_private_key(
            key_file.read(),
            password=None,
            backend=default_backend()
        )
    return private_key


# Створення підпису
def sign_document(document_path, private_key_path):
    private_key = load_private_key(private_key_path)

    with open(document_path, 'rb') as document:
        document_data = document.read()

    # Підписування документа
    signature = private_key.sign(
        document_data,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )

    # Збереження підпису у форматі p7s (CMS)
    signed_file_path = f'{document_path}.p7s'
    with open(signed_file_path, 'wb') as p7s_file:
        p7s_file.write(signature)

    return signed_file_path


# API маршрут для підпису файлу
@app.route('/sign', methods=['POST'])
def sign_file():
    # Перевірка наявності файлів у запиті
    if 'file' not in request.files or 'private_key' not in request.files:
        return jsonify({"error": "No file or private_key provided"}), 400

    file = request.files['file']
    private_key_file = request.files['private_key']

    # Збереження файлів
    filename = secure_filename(file.filename)
    private_key_filename = secure_filename(private_key_file.filename)

    # Створення каталогу для тимчасових файлів, якщо ще не існує
    if not os.path.exists('uploads'):
        os.makedirs('uploads')

    file_path = os.path.join('uploads', filename)
    private_key_path = os.path.join('uploads', private_key_filename)

    file.save(file_path)
    private_key_file.save(private_key_path)

    # Підписуємо файл
    signed_file_path = sign_document(file_path, private_key_path)

    # Повертаємо підписаний файл для завантаження на клієнтську сторону
    return send_file(signed_file_path, as_attachment=True)


if __name__ == '__main__':
    app.run(host='0.0.0.0',debug=True)
