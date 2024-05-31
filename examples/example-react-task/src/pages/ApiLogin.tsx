// import React from 'react'

// export default function ApiLogin() {
//     return (
//         <div>
//             <h1>Hello...</h1>
//         </div>
//     )
// }

import { FormEvent, useState } from 'react';
import { Navigate } from 'react-router-dom';
import { useApp } from '@realm/react';

import logo from '../assets/atlas-app-services.png';
import styles from '../styles/LoginPage.module.css';

/**
 * Screen for logging in to the App Services App using API Key.
 */
export function LoginPage() {
    const atlasApp = useApp();
    const [apiKey, setApiKey] = useState('');
    const [isLoggedIn, setIsLoggedIn] = useState(false);  // State to handle redirection

    // The `currentUser` will be set after a successful login.
    if (atlasApp.currentUser || isLoggedIn) {
        // return <Navigate to='/tasks' />;
        return <Navigate to='/addUsers' />;
    }

    const handleSubmit = async (event: FormEvent<HTMLFormElement>): Promise<void> => {
        event.preventDefault();
        try {
            // Log in using the API key
            const user = await atlasApp.logIn(Realm.Credentials.apiKey(apiKey));
            if (user) {
                // Set state to trigger redirection
                setIsLoggedIn(true);
            }
        } catch (error) {
            console.error("Failed to log in", error);
        }
    };

    return (
        <div className={styles.container}>
            <img
                alt='Atlas Device Sync'
                className={styles.logo}
                src={logo}
            />
            <h1>
                Atlas Device SDK for Web
            </h1>
            <form onSubmit={handleSubmit} className={styles.form}>
                <input
                    className={styles.input}
                    type='text'
                    placeholder='API Key'
                    value={apiKey}
                    onChange={(event) => setApiKey(event.currentTarget.value)}
                />
                <button
                    className={styles.button}
                    type='submit'
                >
                    Log In
                </button>
            </form>
        </div>
    );
}

export default LoginPage;
