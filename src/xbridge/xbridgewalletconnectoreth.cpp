/ Copyright (c) 2017-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <xbridge/xbridgewalletconnectoreth.h>

#include <xbridge/util/logger.h>

//*****************************************************************************
//*****************************************************************************
namespace xbridge
{

namespace
{

// One place to record that a call reached a method with no account-model
// implementation yet. Kept deliberately noisy: a silent false here would be
// indistinguishable from a real failure.
bool notImplemented(const char * what)
{
    LOG() << "ETH connector: " << what << " is not implemented yet";
    return false;
}

} // namespace

//*****************************************************************************
//*****************************************************************************
EthWalletConnector::EthWalletConnector() = default;

//*****************************************************************************
//*****************************************************************************
bool EthWalletConnector::init()
{
    LOG() << "ETH connector loaded for " << currency << " (stub: no operations "
          << "are supported yet)";
    return true;
}

//*****************************************************************************
// Address forms
//*****************************************************************************
std::string EthWalletConnector::fromXAddr(const std::vector<unsigned char> & xaddr) const
{
    if (xaddr.size() != 20)
        return std::string();
    return HexStr(xaddr);
}

std::vector<unsigned char> EthWalletConnector::toXAddr(const std::string & addr) const
{
    std::string hex = addr;
    if (hex.size() > 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
        hex = hex.substr(2);
    if (hex.size() != 40 || !IsHex(hex))
        return std::vector<unsigned char>();
    return ParseHex(hex);
}

//*****************************************************************************
// Chain and wallet queries
//*****************************************************************************
bool EthWalletConnector::getNewAddress(std::string &)
{
    return notImplemented("getNewAddress");
}

bool EthWalletConnector::requestAddressBook(std::vector<wallet::AddressBookEntry> &)
{
    return notImplemented("requestAddressBook");
}

bool EthWalletConnector::getInfo(rpc::WalletInfo &) const
{
    return notImplemented("getInfo");
}

bool EthWalletConnector::getUnspent(std::vector<wallet::UtxoEntry> &,
                                    const std::set<wallet::UtxoEntry> &) const
{
    // An account has no unspent outputs. Answering this at all requires a
    // decision about how the order manager reserves account funds.
    return notImplemented("getUnspent (no account-model equivalent)");
}

bool EthWalletConnector::getBlock(const std::string &, std::string &)
{
    return notImplemented("getBlock");
}

bool EthWalletConnector::getBlockHash(const uint32_t &, std::string &)
{
    return notImplemented("getBlockHash");
}

bool EthWalletConnector::getBlockCount(uint32_t &)
{
    return notImplemented("getBlockCount");
}

bool EthWalletConnector::getTransactionsInBlock(const std::string &,
                                                std::vector<std::string> &)
{
    return notImplemented("getTransactionsInBlock");
}

bool EthWalletConnector::getTxOut(wallet::UtxoEntry &)
{
    return notImplemented("getTxOut (no account-model equivalent)");
}

bool EthWalletConnector::sendRawTransaction(const std::string &, std::string &,
                                            int32_t & errorCode, std::string & message)
{
    errorCode = -1;
    message = "ETH connector is a stub";
    return notImplemented("sendRawTransaction");
}

bool EthWalletConnector::signMessage(const std::string &, const std::string &,
                                     std::string &)
{
    return notImplemented("signMessage");
}

bool EthWalletConnector::verifyMessage(const std::string &, const std::string &,
                                       const std::string &)
{
    return notImplemented("verifyMessage");
}

bool EthWalletConnector::getRawMempool(std::vector<std::string> &)
{
    return notImplemented("getRawMempool");
}

//*****************************************************************************
// Addresses and keys
//*****************************************************************************
bool EthWalletConnector::hasValidAddressPrefix(const std::string & addr) const
{
    return isValidAddress(addr);
}

bool EthWalletConnector::isValidAddress(const std::string & addr) const
{
    return !toXAddr(addr).empty();
}

bool EthWalletConnector::isDustAmount(const double &) const
{
    // Account chains have no dust threshold.
    return false;
}

bool EthWalletConnector::newKeyPair(std::vector<unsigned char> &,
                                    std::vector<unsigned char> &)
{
    return notImplemented("newKeyPair");
}

std::vector<unsigned char> EthWalletConnector::getKeyId(const std::vector<unsigned char> &)
{
    notImplemented("getKeyId");
    return std::vector<unsigned char>();
}

std::vector<unsigned char> EthWalletConnector::getScriptId(const std::vector<unsigned char> &)
{
    notImplemented("getScriptId (no scripts on an account chain)");
    return std::vector<unsigned char>();
}

std::string EthWalletConnector::scriptIdToString(const std::vector<unsigned char> &) const
{
    return std::string();
}

double EthWalletConnector::minTxFee1(const uint32_t, const uint32_t) const
{
    // Gas is not a function of input and output counts.
    return 0.0;
}

double EthWalletConnector::minTxFee2(const uint32_t, const uint32_t) const
{
    return 0.0;
}

//*****************************************************************************
// Swap verification
//*****************************************************************************
bool EthWalletConnector::checkDepositTransaction(const std::string &, const std::string &,
                                                 double &, uint64_t &, uint32_t &,
                                                 const std::string &, double &,
                                                 bool & isGood)
{
    isGood = false;
    return notImplemented("checkDepositTransaction");
}

bool EthWalletConnector::getSecretFromPaymentTransaction(const std::string &,
                                                         const std::string &,
                                                         const uint32_t &,
                                                         const std::vector<unsigned char> &,
                                                         std::vector<unsigned char> &,
                                                         bool & isGood)
{
    isGood = false;
    return notImplemented("getSecretFromPaymentTransaction");
}

uint32_t EthWalletConnector::lockTime(const char) const
{
    notImplemented("lockTime");
    return 0;
}

bool EthWalletConnector::acceptableLockTimeDrift(const char, const uint32_t) const
{
    return false;
}

//*****************************************************************************
// Transaction construction
//*****************************************************************************
bool EthWalletConnector::createDepositUnlockScript(const std::vector<unsigned char> &,
                                                   const std::vector<unsigned char> &,
                                                   const std::vector<unsigned char> &,
                                                   const uint32_t,
                                                   std::vector<unsigned char> &)
{
    return notImplemented("createDepositUnlockScript (no scripts on an account chain)");
}

bool EthWalletConnector::createDepositTransaction(const std::vector<XTxIn> &,
                                                  const std::vector<std::pair<std::string, double> > &,
                                                  std::string &, uint32_t &, std::string &)
{
    return notImplemented("createDepositTransaction");
}

bool EthWalletConnector::createRefundTransaction(const std::vector<XTxIn> &,
                                                 const std::vector<std::pair<std::string, double> > &,
                                                 const std::vector<unsigned char> &,
                                                 const std::vector<unsigned char> &,
                                                 const std::vector<unsigned char> &,
                                                 const uint32_t, std::string &, std::string &)
{
    return notImplemented("createRefundTransaction");
}

bool EthWalletConnector::createPaymentTransaction(const std::vector<XTxIn> &,
                                                  const std::vector<std::pair<std::string, double> > &,
                                                  const std::vector<unsigned char> &,
                                                  const std::vector<unsigned char> &,
                                                  const std::vector<unsigned char> &,
                                                  const std::vector<unsigned char> &,
                                                  std::string &, std::string &)
{
    return notImplemented("createPaymentTransaction");
}

bool EthWalletConnector::createPartialTransaction(const std::vector<XTxIn>,
                                                  const std::vector<std::pair<std::string, double> >,
                                                  std::string &, std::string &)
{
    return notImplemented("createPartialTransaction");
}

bool EthWalletConnector::splitUtxos(CAmount, std::string, bool,
                                    std::set<wallet::UtxoEntry>, std::set<COutPoint>,
                                    CAmount &, CAmount &, int &,
                                    std::string &, std::string &,
                                    std::string & failReason)
{
    failReason = "ETH connector does not support utxo splitting";
    return notImplemented("splitUtxos (no account-model equivalent)");
}

bool EthWalletConnector::isUTXOSpentInTx(const std::string &, const std::string &,
                                         const uint32_t &, bool & isSpent)
{
    isSpent = false;
    return notImplemented("isUTXOSpentInTx");
}

} // namespace xbridge
