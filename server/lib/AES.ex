defmodule AES do

  def encrypt(plainText, key, ivSize \\ 16) do
    iv = :crypto.strong_rand_bytes(ivSize)
    state = :crypto.stream_init(:aes_ctr, key, iv)

    {_state, ciphertext} = :crypto.stream_encrypt(state, plainText)
    iv <> ciphertext
  end

  def decrypt(cipherText, key, ivSize \\ 16) do
    <<iv::binary-size(ivSize), cipherText::binary>> = cipherText
    state = :crypto.stream_init(:aes_ctr, key, iv)
    {_state, plaintext} = :crypto.stream_decrypt(state, cipherText)
    plaintext
  end
end
