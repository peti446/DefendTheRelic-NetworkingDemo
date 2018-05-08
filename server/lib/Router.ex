defmodule Router do
  use GenServer

  @pName :s6073335_Router

  def start_link() do
    GenServer.start_link(__MODULE__, :ok, name: @pName)
  end

  def init(:ok) do
    {:ok, %{nextRegID: 0, prefixReg: Utility.random_string(6), players: %{}, games: %{}, lobbies: %{}, rsa: RSA.generate_keyPair()}}
  end

  def terminate(reason, state) do
    IO.puts "Router closed, reason #{inspect reason} with state #{inspect state}"
  end

  #API Calls
  def stop(reason \\ :normal, timeout \\ :infinity) do
    GenServer.stop(@pName, reason, timeout)
  end

  def server_handle_msg(msg) do
    GenServer.call(@pName, msg)
  end

  def server_user_disconected(name) do
    GenServer.cast(@pName, {:tcp_conn_closed, name})
  end

  #Handle messages functions

  ##Cast Handles
  def handle_cast({:tcp_conn_closed, name}, state) do
    IO.puts("Trying to destroy #{inspect name}")
    case Map.pop(state.players, name) do
       {nil, _} ->
        IO.puts("Could not destroy it")
        {:noreply, state}
      {_, newmap} ->
        state = Map.replace!(state, :players, newmap)
        IO.puts "Destroyed!"
        {:noreply, state}
    end
  end

  def handle_cast(msg, state) do
    IO.puts("Could not handle cast message #{inspect msg}")
    {:noreply, state}
  end

  ##Call handles
  def handle_call({socket, [userID, "1", encrpted]}, from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        {:ok, aesCipher} = Base.decode64(encrpted, ignore: :whitespace)
        {:ok, aesKey} = Map.fetch(userMap, :aesKey)
        str = AES.decrypt(aesCipher, aesKey)
        Router.handle_call({socket,[userID | Utility.networkStringSplitter(str)]}, from, state)
      :error ->
        IO.puts("Recived encrypted message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({socket, [userID, "2", newName]}, _from, state) do

    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        duplicated = Enum.any?(state.players, fn {k , v} ->
          v.display_name == newName
        end)
        case duplicated do
          true ->
            TCP.send_tpc_message(socket, 0, Utility.add_header_to_str(""));
            {:reply ,state, state}
          false->
            userMap = Map.put(userMap, :display_name, newName);
            playerMap = Map.put(state.players, userID, userMap)
            state = Map.put(state, :players, playerMap)
            TCP.send_tpc_message(socket, 2, Utility.add_header_to_str(newName));
            {:reply ,state, state}
        end
      :error ->
        IO.puts("Could not change the displayname to #{inspect newName} from #{inspect userID}. Player is not regisrted????? This should not be possible!! Maybe MIA??")
        {:reply ,state, state}
    end
  end

  def handle_call({socket, ["st", "hsk"]}, _from, state) do
    {pubstr, _} = state.rsa
    TCP.send_tpc_message(socket, Utility.add_header_to_str(Base.encode64(pubstr)))
    {:reply ,state, state}
  end

  def handle_call({socket, address, port, ["notify", "me", "server"]}, _from, state) do
    UDP.send_udp_msg(socket, address, port, <<TCP.port::big-16>>)
    {:reply ,state, state}
  end

  def handle_call({socket, ["aesReg", encryptedKey]}, _from, state) do
      ##Get the aes key and IV
      {_, pri} = state.rsa;
      {:ok, rsaChipher} = Base.decode64(encryptedKey, ignore: :whitespace)
      [key, udpAddress, udpPort] = Utility.networkStringSplitter(RSA.decrypt(rsaChipher, pri))

      ##Get the current ID and increase it by on
      {currentID, newMap} = Map.get_and_update(state, :nextRegID, fn current_value -> {current_value, current_value+1} end)
      currentID = state.prefixReg <>  "_" <> to_string(currentID);
      state = newMap
      ##Create a player entri in the map
      displayName = Utility.random_string_32encode(8)
      state = Kernel.put_in(state, [:players, currentID], %{tcp_socker: socket, udp_address: udpAddress, udp_port: udpPort, aesKey: key, display_name: displayName})

      ##For debug porpuses
      IO.puts("Registered user: #{inspect currentID}")

      ##Send back the tcp response
      TCP.send_tpc_message(socket, Utility.add_header_to_str(Base.encode64(AES.encrypt("ok-::-" <>  currentID <> "-::-" <> displayName, key))))
      {:reply , {:user_rg, currentID}, state}
  end

  def handle_call(msg, _from, state) do
    IO.puts("Could not handle call message #{inspect msg}")
    {:reply ,state, state}
  end
end
