defmodule TCP do
  @tcp_port 13002
  def port, do: @tcp_port

  def start_link() do
    options = [:binary, reuseaddr: true, active: :once, backlog: 5]
    case :gen_tcp.listen(@tcp_port, options) do
  		{:ok, socket} ->
  			pid = spawn(__MODULE__, :accept_loop, [socket])
        IO.puts("Started TCP server over the port #{inspect @tcp_port} and options #{inspect options}")
        {:ok, pid}
      {:error, reason} ->
  			IO.puts("Could not listen on tcp port #{inspect @tcp_port} with options #{inspect options}. Returned error: #{inspect reason}")
        {:error, "Error starting TCP server (Start listening)"}
    end
  end

  def accept_loop(listening_socket) do
    case :gen_tcp.accept(listening_socket) do
      {:ok, client_socket} ->
        IO.puts("Socket #{inspect client_socket} connected ok. Creating child process")

        case TcpSupervisor.add_child([client_socket]) do
          {:ok, pid} ->
            :gen_tcp.controlling_process(client_socket, pid)
            accept_loop(listening_socket)
        end
      {:error, reason} ->
        IO.puts("Could not accept socket reason: #{inspect reason}")
        throw("Error acceping connection")
    end
  end

##Client loops to accept messages
  def start_tcp_client_loop(client_socket) do
    pid = spawn(__MODULE__, :tcp_client_loop, [client_socket])
    {:ok, pid}
  end

	def tcp_client_loop(socket, name \\ "") do
    receive do
     {:tcp, ^socket, data} ->
         :inet.setopts(socket, [active: :once])
         messageList = Utility.networkStringSplitter(Utility.packetToString(Utility.packetToString(data)));
         case Router.server_handle_msg({socket, messageList}) do
          {:user_rg, usrname}->
            tcp_client_loop(socket, usrname)
          _->
            tcp_client_loop(socket, name)
         end
     {:tcp_closed, ^socket} ->
          IO.puts("Socket closing #{inspect socket}")
          if(String.trim(name) != "") do
            Router.server_user_disconected(name)
          end
     _ ->
          IO.puts("Could not handle msg")
    end
  end


  ##API to be accesed by the client
    def send_tpc_message(socket, msg) do
      :gen_tcp.send(socket, Utility.add_header_to_str(msg))
    end

    def send_tpc_message(socket, type, msg) do
      :gen_tcp.send(socket, Utility.add_header_to_str(<<type::unsigned-big-integer-16>> <> msg))
    end

    def send_tpc_encrypted_message(socket, msg, aesKey) do
      :gen_tcp.send(socket, Utility.add_header_to_str(<<1::unsigned-big-integer-16>> <>
                                                      Utility.add_header_to_str(
                                                        Base.encode64(
                                                          AES.encrypt(
                                                            msg,
                                                            aesKey
                                                          )
                                                        )
                                                      )
                            )
                    )
    end

    def send_tpc_encrypted_message(socket, type, msg, aesKey) do
      :gen_tcp.send(socket, Utility.add_header_to_str(<<1::unsigned-big-integer-16>> <>
                                                        Utility.add_header_to_str(
                                                          Base.encode64(
                                                            AES.encrypt(
                                                              <<type::unsigned-big-integer-16>> <> msg,
                                                              aesKey
                                                            )
                                                          )
                                                        )
                                                      )
                    )
    end
end
