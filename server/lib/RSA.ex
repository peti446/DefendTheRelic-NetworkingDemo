defmodule RSA do

  def decrypt(data, privatekey) do
    :public_key.decrypt_private(data, RSA.getKeyFromPem(privatekey))
  end

  def encrypt(data, publickey) do
    :public_key.encrypt_public(data, RSA.getKeyFromPem(publickey))
  end
##'rsa_pkcs1_oaep_padding'
  def getKeyFromPem(key_str) do
    [entry] = :public_key.pem_decode(key_str)
    :public_key.pem_entry_decode(entry)
  end

  def generate_keyPair(key_lenght \\ 2048) do

    #Name of the keys
    publicKey = "peti446_PublicKey.pem"
    privateKey = "peti446_PrivateKey.pem"

    #Generate the keys using openssl
    {_, 0} = System.cmd "openssl", [ "genrsa", "-out", privateKey, to_string(key_lenght)], [stderr_to_stdout: true]
    {_, 0} = System.cmd "openssl", [ "rsa", "-pubout", "-in" , privateKey, "-out", publicKey ], [stderr_to_stdout: true]
    ##Read the keys
    {:ok, privateKey_str} = File.read(privateKey)
    {:ok, publicKey_str} = File.read(publicKey)

    ##Remove the files
    File.rm!(publicKey)
    File.rm!(privateKey)

    # Return tuple for key pair
    {publicKey_str, privateKey_str}
  end
end
