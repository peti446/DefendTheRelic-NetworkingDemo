defmodule UDP do

  @udp_port 13001
  def port, do: @udp_port

  def start_link() do
    options = [:binary, reuseaddr: true, active: false]
    case :gen_udp.open(@udp_port, options) do
        {:ok, socket} ->
          pid = spawn(__MODULE__, :recv_loop, [socket])
          IO.puts("Started UDP over the port #{inspect @udp_port} with options #{inspect options}")
          {:ok, pid}
        {:error, reason} ->
          IO.puts("Could not start the UDP socket, reason #{inspect reason}")
          {:error, "Error starting UDP server (Opening)"}
    end
  end

  def recv_loop(socket) do
    case :gen_udp.recv(socket, 0) do
      {:ok, {address, port, message}}  ->
        messageList = Utility.networkStringSplitter(Utility.packetToString(message));
        Router.server_handle_msg({socket, address, port, messageList})
        recv_loop(socket)
      {:error, reason} ->
        IO.puts "Udp server failed:"
        IO.inspect reason
        :gen_udp.close(socket)
        throw("UDP error trying to recive")
    end
  end


## Server Api function calls

  def send_udp_message(socket, address, port, msg) do
    :gen_udp.send(socket, address, port, msg)
  end

  def send_udp_message(socket, address, port, type, msg) do
    :gen_udp.send(socket, address, port, <<type::unsigned-big-integer-16>> <> msg)
  end

  def send_udp_encrypted_message(socket, address, port, msg, aesKey) do
    :gen_udp.send(socket, address, port, <<1::unsigned-big-integer-16>> <>
                                          Utility.add_header_to_str(
                                            Base.encode64(
                                              AES.encrypt(
                                                msg,
                                                aesKey
                                              )
                                            )
                                          )
                  )
  end

  def send_udp_encrypted_message(socket, address, port, type, msg, aesKey) do
    :gen_udp.send(socket, address, port, <<1::unsigned-big-integer-16>> <>
                                          Utility.add_header_to_str(
                                            Base.encode64(
                                              AES.encrypt(
                                                <<type::unsigned-big-integer-16>> <> msg,
                                                aesKey
                                              )
                                            )
                                          )
                  )
  end
end
