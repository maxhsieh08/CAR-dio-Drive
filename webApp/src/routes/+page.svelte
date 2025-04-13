<script>
    let username = '';
    let password = '';
    let isLogin = true;
  
    // @ts-ignore
    function handleSubmit(event) {
      event.preventDefault();
      if (isLogin) {
        console.log('Login attempted:', { username });
      } else {
        console.log('Registration attempted:', { username });
      }
    }
  </script>
  
  <header class="site-header">
    <nav class="nav-container">
      <div class="logo">
        <h1>CAR-dio Drive</h1>
      </div>
      <div class="nav-links">
        <a href="/" class="active">Home</a>
        <a href="/about">About</a>
        <a href="/contact">Contact</a>
      </div>
    </nav>
  </header>
  
  <main class="container">
    <div class="auth-container">
      <div class="auth-tabs">
        <button 
          class:active={isLogin} 
          on:click={() => isLogin = true}
        >
          Login
        </button>
        <button 
          class:active={!isLogin} 
          on:click={() => isLogin = false}
        >
          Create Account
        </button>
      </div>
  
      <form class="auth-form" on:submit={handleSubmit}>
        <div class="form-group">
          <label for="username">Username</label>
          <input 
            type="text" 
            id="username" 
            bind:value={username} 
            placeholder="Enter your username"
            required 
          />
        </div>
        
        <div class="form-group">
          <label for="password">Password</label>
          <input 
            type="password" 
            id="password" 
            bind:value={password} 
            placeholder="Enter your password"
            required 
          />
        </div>
    
        <button type="submit">
          {isLogin ? 'Log In' : 'Create Account'}
        </button>
      </form>
    </div>
  </main>
  
  <style>
    .site-header {
      background-color: #2c3e50;
      padding: 1rem 0;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      position: sticky;
      top: 0;
      z-index: 1000;
    }
  
    .nav-container {
      max-width: 1200px;
      margin: 0 auto;
      padding: 0 2rem;
      display: flex;
      justify-content: space-between;
      align-items: center;
    }
  
    .logo h1 {
      color: white;
      margin: 0;
      font-size: 1.8rem;
      font-weight: 700;
      text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2);
    }
  
    .nav-links {
      display: flex;
      gap: 2rem;
    }
  
    .nav-links a {
      color: #ecf0f1;
      text-decoration: none;
      font-weight: 500;
      font-size: 1.1rem;
      padding: 0.5rem 1rem;
      border-radius: 4px;
      transition: all 0.3s ease;
    }
  
    .nav-links a:hover {
      background-color: rgba(255, 255, 255, 0.1);
    }
  
    .nav-links a.active {
      background-color: #3498db;
    }
  
    .container {
      max-width: 450px;
      margin: 3rem auto;
      padding: 2rem;
      text-align: center;
    }
  
    .auth-container {
      background: white;
      border-radius: 12px;
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.1);
      padding: 2rem;
    }
  
    .auth-tabs {
      display: flex;
      gap: 1rem;
      margin-bottom: 2rem;
    }
  
    .auth-tabs button {
      flex: 1;
      background: transparent;
      border: none;
      padding: 0.75rem;
      font-size: 1rem;
      color: #666;
      cursor: pointer;
      border-bottom: 2px solid transparent;
      transition: all 0.3s ease;
    }
  
    .auth-tabs button.active {
      color: #3498db;
      border-bottom: 2px solid #3498db;
      font-weight: bold;
    }
  
    .auth-form {
      display: flex;
      flex-direction: column;
      gap: 1.5rem;
    }
  
    .form-group {
      display: flex;
      flex-direction: column;
      text-align: left;
      gap: 0.5rem;
    }
  
    label {
      font-weight: 600;
      color: #2c3e50;
      font-size: 0.9rem;
    }
  
    input {
      padding: 0.75rem;
      border: 2px solid #e0e0e0;
      border-radius: 8px;
      font-size: 1rem;
      transition: border-color 0.3s ease;
    }
  
    input:focus {
      outline: none;
      border-color: #3498db;
      box-shadow: 0 0 0 2px rgba(52, 152, 219, 0.1);
    }
  
    input::placeholder {
      color: #bbb;
    }
  
    button[type="submit"] {
      background-color: #3498db;
      color: white;
      padding: 0.75rem;
      border: none;
      border-radius: 8px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      margin-top: 1rem;
      transition: background-color 0.3s ease;
    }
  
    button[type="submit"]:hover {
      background-color: #2980b9;
    }
  
    @media (max-width: 480px) {
      .nav-container {
        padding: 0 1rem;
        flex-direction: column;
        gap: 1rem;
      }
  
      .nav-links {
        gap: 1rem;
      }
  
      .nav-links a {
        font-size: 0.9rem;
        padding: 0.4rem 0.8rem;
      }
  
      .container {
        margin: 1rem auto;
        padding: 1rem;
      }
      
      .auth-container {
        padding: 1.5rem;
      }
    }
  </style>