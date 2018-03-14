defmodule TCP do
  def start_link() do
    port = 13000
    options = [:binary, reuseaddr: true, active: :once, backlog: 5]
  	case :gen_tcp.listen(port, options) do
  		{:ok, socket} ->
  			accept_loop(socket)
  		{:error, reason} ->
  			IO.puts("Could not listen on tcp port #{inspect port} with options #{inspect options}. Returned error: #{inspect reason}")
        throw("Error when starting to listening on port")
    end
  end

  def accept_loop(listening_socket) do
    case :gen_tcp.accept(listening_socket) do
      {:ok, client_socket} ->
        TcpSupervisor.add_child([client_socket])
        accept_loop(listening_socket)
      {:error, reason} ->
        IO.puts("Could not accept socket reason: #{inspect reason}")
        throw("Error acceping connection")
    end
  end

	def tcp_client_loop(socket) do
	 receive do
	   {:tcp, socket, data} ->
		     :inet.setopts(socket, [active: :once])
         tcp_client_loop(socket)
	   {:tcp_closed, socket} ->
          IO.puts("Socket closing #{inspect socket}")
	 end
 end
end
